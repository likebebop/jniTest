#include <jni.h>
#include <string>
#include "jniHelper.hpp"

using namespace std;

extern "C" {


JNIEXPORT jfloat JNICALL
Java_com_example_likebebop_jnitest_JniTest_getArrayFieldFromNative(JNIEnv *env, jobject instance,
                                                                    jobject obj) {

    float result = 0.0f;

    jclass cls = env->GetObjectClass(obj);

    std::pair<jobject, float*> array = JniHelper::getFloatArray(env, cls, obj, "floatArray");

    jsize len = env->GetArrayLength(*reinterpret_cast<jfloatArray *>(&array.first));

    for (int i = 0; i < len; ++i) {
        result += array.second[i];
    }

    JniHelper::releaseFloatArray(env, array);

    /*result = 0.0;
    // get field [F = Array of float
    jfieldID fieldId = env->GetFieldID(cls, "floatArray", "[F");

    // Get the object field, returns JObject (because Array is instance of Object)
    jobject objArray = env->GetObjectField(obj, fieldId);

    // Cast it to a jfloatarray
    jfloatArray *fArray = reinterpret_cast<jfloatArray *>(&objArray);

    len = env->GetArrayLength(*fArray);

    // Get the elements
    float *data = env->GetFloatArrayElements(*fArray, 0);

    for (int i = 0; i < len; ++i) {
        result += data[i];
    }

    // Don't forget to release it
    env->ReleaseFloatArrayElements(*fArray, data, 0);*/

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
Java_com_example_likebebop_jnitest_JniTest_getFieldFromNative(JNIEnv *env, jobject instance,
                                                          jobject obj) {

    jclass cls = env->GetObjectClass(obj);
    jint num = env->GetIntField(obj, env->GetFieldID(cls, "arraySize", "I"));
    return num;
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
