#include <jni.h>
#include <string>


using namespace std;
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_likebebop_jnitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
