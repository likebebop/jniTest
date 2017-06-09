#include "jniHelper.hpp"
#include <android/log.h>

using namespace JniHelper;
static const char* TAG = "likebebop";

FloatArrayWrapper::~FloatArrayWrapper() {
    env->ReleaseFloatArrayElements(*reinterpret_cast<jfloatArray *>(&arrayObj), array, 0);
    __android_log_print(ANDROID_LOG_INFO, TAG, "~FloatArrayWrapper");
}

FloatArrayWrapper::FloatArrayWrapper(JNIEnv *env, jclass clazz, jobject obj,
                                     const char *name) : env(env) {
    __android_log_print(ANDROID_LOG_INFO, TAG, "FloatArrayWrapper");
    jfieldID fid = env->GetFieldID(clazz, name, "[F");
    arrayObj = env->GetObjectField(obj, fid);
    jfloatArray* pFloatArray = reinterpret_cast<jfloatArray *>(&arrayObj);
    array = env->GetFloatArrayElements(*pFloatArray, 0);
}

