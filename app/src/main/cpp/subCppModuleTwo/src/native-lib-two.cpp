#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_inin_nativego_MainActivity_catStr(JNIEnv *env, jobject instance, jstring old_,
                                           jint numbers, jobject strs, jobject maps) {
    const char *old = env->GetStringUTFChars(old_, 0);


//        env->NewLocalRef()
//        env->NewObject()
//        env->NewIntArray()
//        env->NewGlobalRef()
//        env->NewWeakGlobalRef()

//        env->DeleteLocalRef()
//        env->DeleteGlobalRef()
//        env->DeleteWeakGlobalRef()

//    env->ExceptionCheck();
//    env->ExceptionOccurred();
//    env->ExceptionDescribe();
//    env->ExceptionClear();


    return env->NewStringUTF(old);
}

//异常处理
//1、当调用一个JNI函数后，必须先检查、处理、清除异常后再做其它 JNI 函数调用，否则会产生不可预知的结果。
//2、一旦发生异常，立即返回，让调用者处理这个异常。或 调用 ExceptionClear 清除异常，然后执行自己的异常处理代码。
//3、异常处理的相关JNI函数总结：
//1> ExceptionCheck：检查是否发生了异常，若有异常返回JNI_TRUE，否则返回JNI_FALSE
//2> ExceptionOccurred：检查是否发生了异常，若用异常返回该异常的引用，否则返回NULL
//3> ExceptionDescribe：打印异常的堆栈信息
//4> ExceptionClear：清除异常堆栈信息
//5> ThrowNew：在当前线程触发一个异常，并自定义输出异常信息
//        jint (JNICALL *ThrowNew) (JNIEnv *env, jclass clazz, const char *msg);
//6> Throw：丢弃一个现有的异常对象，在当前线程触发一个新的异常
//        jint (JNICALL *Throw) (JNIEnv *env, jthrowable obj);
//7> FatalError：致命异常，用于输出一个异常信息，并终止当前VM实例（即退出程序）
//void (JNICALL *FatalError) (JNIEnv *env, const char *msg);
