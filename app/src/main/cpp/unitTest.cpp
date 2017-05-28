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
            My::debug("Derived");
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

        ~Derived() {
            My::debug("~Derived");
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

    void testUniquePtr() {
        unique_ptr<int> p1(new int(5));

        // 복사생성이 허용되어 있지 않다. 컴파일 에러
        //unique_ptr<int> p2 = p1;

        // 소유권 이전 (p1 -> p3)
        unique_ptr<int> p3 = std::move(p1);

        // 메모리 해제
        // 굳이 이걸 호출하지 않더라도 스코프 종료시 메모리 해제된다
        //p3.reset();

        // 아무 것도 수행치 않는다
        //p1.reset();


    }

    void foo(const std::shared_ptr<Derived>& d) {
        d.get()->debug();
        d.get()->name = "shared in foo";
    }

    void testSharedPtr() {
        auto sp = std::make_shared<Derived>(1, "before shared", 1);
        sp->debug();
        foo(sp);
        sp->debug();
    }

}



extern "C" {

using namespace Test;

JNIEXPORT void JNICALL
Java_com_example_likebebop_jnitest_JniTest_testAll(JNIEnv *env, jobject instance) {

    testString();
    testStruct();
    testClass();
    testUniquePtr();
    testSharedPtr();

}


};