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
        jobject /* this */) {
    string hello = "Hello from C++";

    json test;

    json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    //__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "happy = %d", j["happy"]);

    __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "happy = %d", false);
    // j["happy"] = true;

    __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "happy = %d, %.3f", (bool) j["happy"],
                        (float) j["pi"]);

    __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "happy =  %.3f", j.at("pi").get<float>());

    return env->NewStringUTF(j.dump().c_str());
}
/*
JNIEXPORT jfloat JNICALL
Java_com_example_likebebop_jnitest_JniTest_getMemberFieldFromNative(JNIEnv *env, jobject instance,
                                                                    jobject obj) {

    // TODO

}

JNIEXPORT jint JNICALL
Java_com_example_likebebop_jnitest_JniTest_invokeMemberFuncFromNative(JNIEnv *env, jobject instance,
                                                                      jobject obj) {

    // TODO

}

JNIEXPORT jobject JNICALL
Java_com_example_likebebop_jnitest_JniTest_createObjectFromNative(JNIEnv *env, jobject instance,
                                                                  jint param) {

    // TODO

}

JNIEXPORT jint JNICALL
Java_com_example_likebebop_jnitest_JniTest_processObjectArrayFromNative(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobjectArray objArray) {

    // TODO

}*/
}
