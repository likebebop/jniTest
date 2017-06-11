#include <jni.h>
#include <string>
#include "jniHelper.hpp"

using namespace std;
using namespace JniHelper;

extern "C" {


JNIEXPORT jfloat JNICALL
Java_com_example_likebebop_jnitest_JniTest_getArrayFieldFromNative(JNIEnv *env, jobject instance,
                                                                    jobject obj) {

    float result = 0.0f;

    jclass cls = env->GetObjectClass(obj);
    FloatArrayWrapper array(env, cls, obj, "floatArray");

    jsize len = env->GetArrayLength(*reinterpret_cast<jfloatArray *>(&array.arrayObj));

    for (int i = 0; i < len; ++i) {
        result += array.array[i];
    }

    float test[len];

    for (int i = 0; i < len; ++i) {
        test[i] = i;
    }

    memcpy(array.array, test, sizeof(test));
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
