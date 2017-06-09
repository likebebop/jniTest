/**
 *
 * JniHelper
 * @author likebebop
 *
 */

#ifndef JniHelper_hpp
#define JniHelper_hpp

#include <jni.h>
#include <utility>
#include <stddef.h>

namespace JniHelper {
    inline int getIntField(JNIEnv *env, jclass clazz, jobject obj, const char* name) {
        return env->GetIntField(obj, env->GetFieldID(clazz, name, "I"));
    }

    inline float getFloatField(JNIEnv *env, jclass clazz, jobject obj, const char* name) {
        return env->GetFloatField(obj, env->GetFieldID(clazz, name, "F"));
    }

    inline void setFloatField(JNIEnv *env, jclass clazz, jobject obj, const char* name, float value) {
        env->SetFloatField(obj, env->GetFieldID(clazz, name, "F"), value);
    }

    inline bool getBooleanField(JNIEnv *env, jclass clazz, jobject obj, const char* name) {
        return env->GetBooleanField(obj, env->GetFieldID(clazz, name, "Z"));
    }

    inline std::pair<jobject, float*> getFloatArray(JNIEnv *env, jclass clazz, jobject obj, const char* name) {
        jfieldID fid = env->GetFieldID(clazz, name, "[F");
        jobject objArray = env->GetObjectField(obj, fid);
        jfloatArray* pFloatArray = reinterpret_cast<jfloatArray *>(&objArray);
        float* pFloat = env->GetFloatArrayElements(*pFloatArray, 0);
        return std::make_pair(objArray, pFloat);
    };

    inline jfloatArray& pairToFloatArray(std::pair<jobject , float*>& array) {
        return *reinterpret_cast<jfloatArray *>(&array.first);
    }

    inline void releaseFloatArray(JNIEnv *env, std::pair<jobject , float*>& array) {
        env->ReleaseFloatArrayElements(*reinterpret_cast<jfloatArray *>(&array.first), array.second, 0);
    }
}

#endif /* JniHelper_hpp */
