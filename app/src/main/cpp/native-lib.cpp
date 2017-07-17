#include <jni.h>
#include <string>
#include <android/log.h>


#include "json.hpp"

using namespace std;
using json = nlohmann::json;


extern "C" {


JNIEXPORT jstring JNICALL
Java_com_example_likebebop_jnitest_JniTest_stringFromJNI(
        JNIEnv *env,
        jobject obj) {
    string hello = "Hello from C++";
    jclass cls;
    jmethodID mid;
    cls = env->FindClass("com/example/likebebop/jnitest/JniTest");
    mid = env->GetMethodID(cls, "getStringFromJava", "(I)Ljava/lang/String;");
    jstring str = (jstring) env->CallObjectMethod(obj, mid, 5);
    const char *s = env->GetStringUTFChars(str, 0);
    env->ReleaseStringUTFChars(str, s);
    return env->NewStringUTF(s);
}
}




