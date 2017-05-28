#include <jni.h>
#include <string>
#include <android/log.h>


#include "json.hpp"

using namespace std;

namespace My {
    void debug(const char* s) {
        __android_log_print(ANDROID_LOG_INFO, "likebebop", s, "");
    }

    void debug(string& str) {
        debug(str.c_str());
    }
}

extern "C" {

using namespace My;

JNIEXPORT void JNICALL
Java_com_example_likebebop_jnitest_JniTest_testAll(JNIEnv *env, jobject instance) {

    cout << "===== oops..... =======" << endl;

    debug("==== 웁스 =====");
    string s = "aa";

    debug(s.append(" bb"));
    //debugS(s += " bb");
    //__android_log_print(ANDROID_LOG_INFO, "likebebop", "==== 웁스 =====");

}


};