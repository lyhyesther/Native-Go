#include <jni.h>
#include <string>
#include <android/log.h>

/**
 * 使用Android NDK 的步骤：
 * 1、先导入要使用API的头文件
 * 2、直接使用
 * */
extern "C" JNIEXPORT jstring JNICALL
Java_com_inin_nativego_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj) {
    //1 获取Java 类对象
    jclass _jclaz = env->GetObjectClass(obj);
    //2 获取 方法Id 参数1 Java类对象；参数2 方法名称；参数3 方法签名
    jmethodID _jmethodId = env->GetMethodID(_jclaz, "getStringInJava", "()Ljava/lang/String;");
    jfieldID _jfieldId = env->GetFieldID(_jclaz, "Id", "I");
    //3 JNI 调用Java的方法
    jstring strFromJavaCode = (jstring) env->CallObjectMethod(obj, _jmethodId);
    //获取int 类型的域
    jint id = env->GetIntField(obj, _jfieldId);
    char *chars = (char *) env->GetStringUTFChars(strFromJavaCode, NULL);
    //将int转化为string 类型
    std::string idStr = std::to_string(id);
    char *achars = strcat(chars, idStr.c_str());
//    std::string hello = "Hello from C++";

    //获取域ID
//        env->GetFieldID()
    //获取域
//        env->GetIntField() //获取Int 类型的域
//        env->GetDoubleField()//获取Double类型的域
//        env->GetShortField()//获取Short类型的域
//        env->GetObjectField()//获取对象类型的域
    //获取静态域
//        env->GetStaticFieldID()//获取静态域ID
//        env->GetStaticIntField()//获取静态Int类型的域
//        env->GetStaticObjectField() //获取静态对象类型的域

    //获取方法Id
//        env->GetMethodID()
//        env->CallObjectMethod()//调用对象方法
//        env->CallIntMethod();//调用返回Int类型的方法
//        env->CallObjectMethod()//调用返回对象类型的方法
    //调用静态方法
//        env->CallStaticObjectMethod()调用返回Object类型的静态方法
//        env->CallStaticIntMethod()//调用返回Int类型的静态方法

    __android_log_print(ANDROID_LOG_DEBUG, "android_native", "");
    return env->NewStringUTF(achars);
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
