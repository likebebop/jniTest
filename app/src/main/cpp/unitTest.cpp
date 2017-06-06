#include <jni.h>
#include <string>
#include <android/log.h>
#include <vector>


using namespace std;

namespace My {
    static const char* TAG = "likebebop";
    void debug(const char *s) {
        __android_log_print(ANDROID_LOG_INFO, TAG, s, "");
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

        //-- https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const
        static constexpr float MY_CONT_FLOAT = 5;
        static int sInt;

        string name;
        float percentage;

        //-- float and vector initialization
        float test = 1.5;
        std::vector<int> v{1, 3, 5};

        //-- static const vector
        static const std::vector<int> scv;


        void debug() {
            char buf[1024];
            sprintf(buf, "addr : %x, id : %d, name : %s, p : %.2f, test : %.2f, sInt : %d, vSize : %d",
                    this,
                    id, name.c_str(), percentage, test, sInt, v.size());
            My::debug(buf);
            //-- compile error
            //MY_CONT_FLOAT = 1;
            sInt = 5;
        }

        ~Derived() {
            My::debug("~Derived");
        }
    };

    //-- static initialization
    int Derived::sInt = 10;
    const std::vector<int> Derived::scv{1, 3, 5};


    //vector<int> Derived::v{1, 3, 5};



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

    void testLamda() {
        auto func = [](int n) {return n*n;};

        int result = func(5);
        __android_log_print(ANDROID_LOG_INFO, TAG, "test Lamda : %d", result);

        std:for_each(Derived::scv.begin(), Derived::scv.end(), [](auto n){
            __android_log_print(ANDROID_LOG_INFO, TAG, "iteration : %d", n);
        });
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
    testLamda();

}


};