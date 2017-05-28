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

JNIEXPORT jfloat JNICALL
Java_com_example_likebebop_jnitest_JniTest_getMemberFieldFromNative(JNIEnv *env, jobject instance,
                                                                    jobject obj) {

    float result = 0.0f;
    jclass cls = env->GetObjectClass(obj);

    // get field [F = Array of float
    jfieldID fieldId = env->GetFieldID(cls, "floatArray", "[F");

    // Get the object field, returns JObject (because Array is instance of Object)
    jobject objArray = env->GetObjectField(obj, fieldId);

    // Cast it to a jfloatarray
    jfloatArray *fArray = reinterpret_cast<jfloatArray *>(&objArray);

    jsize len = env->GetArrayLength(*fArray);

    // Get the elements
    float *data = env->GetFloatArrayElements(*fArray, 0);

    for (int i = 0; i < len; ++i) {
        result += data[i];
    }

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*fArray, data, 0);

    return result;

}

// utility method
int getArraySize(JNIEnv *env, jobject obj) {
    jclass cls = env->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(cls, "getArraySize", "()I");
    int result = env->CallIntMethod(obj, methodId);

    return result;
}

JNIEXPORT jint JNICALL
Java_com_example_likebebop_jnitest_JniTest_invokeMemberFuncFromNative(JNIEnv *env, jobject instance,
                                                                      jobject obj) {
    int facetCount = getArraySize(env, obj);
    return facetCount;

}

JNIEXPORT jobject JNICALL
Java_com_example_likebebop_jnitest_JniTest_createObjectFromNative(JNIEnv *env, jobject instance,
                                                                  jint param) {

    jclass cls = env->FindClass("com/example/likebebop/jnitest/MeshData");
    jmethodID methodId = env->GetMethodID(cls, "<init>", "(I)V");
    jobject obj = env->NewObject(cls, methodId, param);

    return obj;

}

JNIEXPORT jint JNICALL
Java_com_example_likebebop_jnitest_JniTest_processObjectArrayFromNative(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobjectArray objArray) {

    int resultSum = 0;

    int len = env->GetArrayLength(objArray);


    for (int i = 0; i < len; ++i) {
        jobject obj = (jobject) env->GetObjectArrayElement(objArray, i);
        resultSum += getArraySize(env, obj);
    }

    return resultSum;

}
}
