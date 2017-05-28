#include <jni.h>
#include <string>
#include <android/log.h>


using namespace std;

namespace My {
    void debug(const char *s) {
        __android_log_print(ANDROID_LOG_INFO, "likebebop", s, "");
    }
}

namespace My {

    struct BaseStruct {
        int id;
    };

    struct Student {
        int id;
        string name;
        float percentage;

        void debug() {
            char buf[1024];
            sprintf(buf, "id : %d, name : %s, p : %.2f", id, name.c_str(), percentage);
            My::debug(buf);
        }
    };

    class Base {
    public :
        Base(int id) {
            this->id = id;
        }

        int id;
    };

    class Derived : public Base {
    public:
        Derived(int id, const char* name, float p) : Base(id), name(name), percentage(p) {
        }

        string name;
        float percentage;
        void debug() {
            char buf[1024];
            sprintf(buf, "addr : %x, id : %d, name : %s, p : %.2f",
                    this,
                    id, name.c_str(), percentage);
            My::debug(buf);
        }
    };

    void debug(string &str) {
        debug(str.c_str());
    }
}

namespace Test {
    using namespace My;

    void testString() {
        debug("==== 웁스 =====");
        string s = "aa";
        debug(s.append(" bb"));
    }

    void testStruct() {
        Student s = {1, "bebop", 0.5};
        s.debug();
    }

    void testClass() {
        Derived s = {1, "bebop", 0.5};
        Derived s2 = s;
        Derived& ref = s;
        s.debug();
        s2.name = "test";
        s2.debug();
        ref.debug();
        ref.name = "hello bebop";
        s.debug();
    }
}



extern "C" {

using namespace Test;

JNIEXPORT void JNICALL
Java_com_example_likebebop_jnitest_JniTest_testAll(JNIEnv *env, jobject instance) {

    testString();
    testStruct();
    testClass();

}


};