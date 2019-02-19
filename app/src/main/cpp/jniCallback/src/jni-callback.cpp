//
// Created by Administrator on 2019/2/18.
//

#include "jni-callback.h"
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <cinttypes>

//Android 日志函数装饰
const char *kTAG = "jni-callback";
#define LOGI(...)  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGD(...)  ((void)__android_log_print(ANDROID_LOG_DEBUG, kTAG, __VA_ARGS__))
#define LOGE(...)  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

//处理Handler类的回调
typedef struct tick_context {
    JavaVM *javaVM;
    jclass jniHelperClz;
    jobject jniHelperObj;
    jclass mainActivityClz;
    jobject mainActivityObj;
    pthread_mutex_t lock;
    int done;
} TickContext;
tick_context g_ctx;


extern "C"
JNIEXPORT jstring JNICALL
Java_com_inin_nativego_jnicallback_JniCallbackActivity_getDeviceARM(JNIEnv *env, jobject instance) {
#if defined(__arm__) //如果定义了宏__arm__
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a/NEON (hard-float)"
#else
#define ABI "armeabi-v7a/NEON"
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#else
#define ABI "armeabi-v7a"
#endif
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86" //定义宏 ABI
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif
    LOGI(" Compiled with ABI "
                 ABI
                 " ");

    // 变量和引号之间要有空格
    return env->NewStringUTF(" Compiled with ABI " ABI " ");
}

/**
 * 一个帮助函数，展示如何调用：
 * Java静态函数 JniHelper::getBuildVersion()
 * Java非静态函数 JniHelper::getRuntimeMemorySize()
 * */
void queryRuntimeInfo(JNIEnv *env, jobject instance) {
    //Java类JniHelper的Id 和 实例 当共享库被加载的时候，就已经初始化，当前可以直接使用
    //静态函数不需要实例，所以对于jni来说，只需要找到类和方法Id
    jmethodID versionMID = env->GetStaticMethodID(g_ctx.jniHelperClz, "getBuildVersion",
                                                  "()Ljava/lang/String;");
    if (!versionMID) {
        LOGE("Failed to retrieve getBuildVersion() methodID @ line %d",
             __LINE__);
        return;
    }
    jstring version = (jstring) env->CallStaticObjectMethod(g_ctx.jniHelperClz, versionMID);

    const char *buildVersion = env->GetStringUTFChars(version, NULL);
    if (!buildVersion) {
        LOGE("Unable to get version string @ line %d", __LINE__);
        return;
    }
    LOGI("Android Version - %s", buildVersion);
    env->ReleaseStringUTFChars(version, buildVersion);
    //该方法是在JNI_OnLoad中调用，所以需要释放LocalRef，来避免弱引用
    env->DeleteLocalRef(version);

    //调用非静态方法，通过JniHelper的一个实例来查询可用内存大小
    jmethodID _getRuntimeMemorySize = env->GetMethodID(g_ctx.jniHelperClz, "getRuntimeMemorySize",
                                                       "()J");
    if (!_getRuntimeMemorySize) {
        LOGE("Failed to retrieve getRuntimeMemorySize() methodID @ line %d",
             __LINE__);
        return;
    }
    jlong avilableMemorySize = env->CallLongMethod(g_ctx.jniHelperObj, _getRuntimeMemorySize);
    LOGI("Runtime free memory size: %"
                 PRId64, avilableMemorySize);
    (void) avilableMemorySize;// silence the compiler warning
}

/**
 * JNI 本身的方法，又JNI本身调用
 * 处理一次性的初始化：
 * 缓存JavaVM 到 自定义的上下文中
 * 找到JniHelper的类ID
 * 创建一个JniHelper的实例
 * 生成一个全局引用，因为会在一个本地线程中使用
 * 注意：
 * 此处分配的所有资源不会由应用（application）释放。当它们不在使用的时候，依赖的系统会释放所有的全局引用
 * 该函数对应的函数JNI_OnUnload()绝不会调用
 * */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    //新申请的内存做初始化工作：将指针&g_ctx所指向的前sizeof(g_ctx)大小的内存都初始化为整数0
    // 第一个参数是void* 型的指针变量，所以它可以为任何类型的数据进行初始化
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    //从JavaVM中获取JNIEnv的实例，void** 二级指针，用来传递指针变量
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;// JNI version not supported.
    }
    //找到某个类的类ID
    jclass clz = env->FindClass("com/inin/nativego/jnicallback/JniHandler");
    g_ctx.jniHelperClz = (jclass) env->NewGlobalRef(clz);
    //获取类的构造器方法
    jmethodID jniHelperCtor = env->GetMethodID(clz, "<init>", "()V");
    jobject handler = env->NewObject(clz, jniHelperCtor);
    g_ctx.jniHelperObj = env->NewGlobalRef(handler);

    queryRuntimeInfo(env, g_ctx.jniHelperObj);
    g_ctx.done = 0;
    g_ctx.mainActivityObj = NULL;
    return JNI_VERSION_1_6;
}

/**
 * 帮助函数，封装JniHelper::updateStatus(String msg)
 * JNI允许通过实例调用函数，尽管该函数是私有函数private
 * */
void sendJavaMsg(JNIEnv *env, jobject instance,
                 jmethodID func, const char *msg) {
    jstring javaMsg = env->NewStringUTF(msg);
    env->CallVoidMethod(instance, func, javaMsg);
    env->DeleteLocalRef(javaMsg);
}

/**
 * 主要的工作线程函数
 * */
void *updateTicks(void *context) {
    LOGD("enter  updateTicks() method.");
    TickContext *pctx = (TickContext *) context;
    LOGD("Get pctx.");
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    LOGD("Get env.");
    if (res != JNI_OK) {
        res = javaVM->AttachCurrentThread(&env, NULL);
        if (JNI_OK != res) {
            LOGD("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }
    LOGD("Success to AttachCurrentThread.");

    jmethodID statusId = env->GetMethodID(pctx->jniHelperClz,
                                          "updateStatus",
                                          "(Ljava/lang/String;)V");
    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: initializing...");

    // get mainActivity updateTimer function
    jmethodID timerId = env->GetMethodID(pctx->mainActivityClz,
                                         "updateTimer", "()V");

    struct timeval beginTime, curTime, usedTime, leftTime;
    const struct timeval kOneSecond = {
            (__kernel_time_t) 1,
            (__kernel_suseconds_t) 0
    };

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: start ticking ...");
    while (1) {
        gettimeofday(&beginTime, NULL);
        pthread_mutex_lock(&pctx->lock);
        int done = pctx->done;
        if (pctx->done) {
            pctx->done = 0;
        }
        pthread_mutex_unlock(&pctx->lock);
        if (done) {
            break;
        }
        env->CallVoidMethod(pctx->mainActivityObj, timerId);

        gettimeofday(&curTime, NULL);
        timersub(&curTime, &beginTime, &usedTime);
        timersub(&kOneSecond, &usedTime, &leftTime);
        struct timespec sleepTime;
        sleepTime.tv_sec = leftTime.tv_sec;
        sleepTime.tv_nsec = leftTime.tv_usec * 1000;

        if (sleepTime.tv_sec <= 1) {
            nanosleep(&sleepTime, NULL);
        } else {
            sendJavaMsg(env, pctx->jniHelperObj, statusId,
                        "TickerThread error: processing too long!");
        }
    }

    sendJavaMsg(env, pctx->jniHelperObj, statusId,
                "TickerThread status: ticking stopped");
    javaVM->DetachCurrentThread();
    return context;
}

/**
 * Java端启动API，调用者可以从onResume中调用
 * */
extern "C"
JNIEXPORT void JNICALL
Java_com_inin_nativego_jnicallback_JniCallbackActivity_startTicks(JNIEnv *env, jobject instance) {
    LOGD("enter startTicks() method");
    //声明线程类型
    pthread_t threadInfo;
    pthread_attr_t threadAttr;

    //创建线程
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&g_ctx.lock, NULL);

    //获取Java层对应的类和对象，并创建全局引用
    jclass jclazz = env->GetObjectClass(instance);
    g_ctx.mainActivityClz = (jclass) env->NewGlobalRef(jclazz);
    g_ctx.mainActivityObj = env->NewGlobalRef(instance);

    LOGD("enter startTicks() method2");

    int result = pthread_create(&threadInfo, &threadAttr, updateTicks, &g_ctx);
//    assert(result == 0);
    //销毁线程
//    pthread_attr_destroy(&threadAttr);
    LOGD("enter startTicks() method3");
}

/**
 *Java端调用停止tricks
 * 返回之前需要确保本地线程finish,并且完全关闭，可以在onPause中调用
 * */
extern "C"
JNIEXPORT void JNICALL
Java_com_inin_nativego_jnicallback_JniCallbackActivity_stopTicks(JNIEnv *env, jobject instance) {
    //线程互斥锁
    pthread_mutex_lock(&g_ctx.lock);
    g_ctx.done = 1;
    pthread_mutex_unlock(&g_ctx.lock);

    // waiting for ticking thread to flip the done flag
    struct timespec sleepTime;
    memset(&sleepTime, 0, sizeof(sleepTime));
    sleepTime.tv_nsec = 100000000;
    while (g_ctx.done) {
        nanosleep(&sleepTime, NULL);
    }

    // release object we allocated from StartTicks() function
    env->DeleteGlobalRef(g_ctx.mainActivityClz);
    env->DeleteGlobalRef(g_ctx.mainActivityObj);
    g_ctx.mainActivityObj = NULL;
    g_ctx.mainActivityClz = NULL;

    pthread_mutex_destroy(&g_ctx.lock);
}



