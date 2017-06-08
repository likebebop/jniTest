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

    enum class DistortionType {
        none = 0, zoom_in, zoom_out, direction, ellipseZoomin, ellipseZoomout, ellipseDirection, uniformZoomInWithoutAdjustment, uniformZoomInWithAdjustment, uniformZoomOut
    };

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
        Derived()  : Base(0) {

        }
        Derived(int id, const char* name, float p) : Base(id), name(name), percentage(p) {
            My::debug("(+)Derived");
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
            My::debug("(-)Derived");
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
        //unique_ptr<int> p1(new int(5));
        __android_log_print(ANDROID_LOG_INFO, TAG, "=== pd begin ===");

        {
            unique_ptr<Derived> pd(new Derived(1, "bebbop", 100));
            __android_log_print(ANDROID_LOG_INFO, TAG, "pd : %.2x", pd.get());
            pd->debug();
        }

        __android_log_print(ANDROID_LOG_INFO, TAG, "=== pd end ===");

        unique_ptr<Derived> pd(new Derived(1, "bebbop2", 200));
        __android_log_print(ANDROID_LOG_INFO, TAG, "pd : %.2x", pd.get());

        vector<unique_ptr<Derived>> v1;

        v1.push_back(std::move(pd));

        __android_log_print(ANDROID_LOG_INFO, TAG, "pd after move : %.2x", pd.get());


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

    //-- c++14
   /* auto multiply() {
        return [](int n){return n*n;};
    }*/

    void testLamda() {
        //-- https://stackoverflow.com/questions/7951377/what-is-the-type-of-lambda-when-deduced-with-auto-in-c11
        //-- 아래 두개 결과가 같다. auto로 파악하는듯
        //auto func = [](auto n)->float{return n*n/2.0;};
        auto func = [&](int n){return n*n/2.0;};
        float result = func(5);
        __android_log_print(ANDROID_LOG_INFO, TAG, "test Lamda : %.2f", result);


       int idx = 7;
       auto func2 = [&idx](int n){return idx*n;};
       auto func3 = [=](int n){return idx*n;};

       __android_log_print(ANDROID_LOG_INFO, TAG, "test lamda %d, %d", func2(10), func3(10));

        //__android_log_print(ANDROID_LOG_INFO, TAG, "test Lamda by function : %d", multiply()(5));
    }

    void testLoop() {
        std:for_each(Derived::scv.begin(), Derived::scv.end(), [](int n){
            __android_log_print(ANDROID_LOG_INFO, TAG, "iteration : %d", n);
        });

        int total = 0;
        for_each(Derived::scv.begin(), Derived::scv.end(), [&total](int n){
            total += n;;
        });
        __android_log_print(ANDROID_LOG_INFO, TAG, "total : %d", total);

        //-- auto
        int idx = 0;
        for (auto i : Derived::scv) {
            __android_log_print(ANDROID_LOG_INFO, TAG, "by auto : %d, %.2x, %.2x", i, &i, &Derived::scv[idx]);
            idx++;
        }

        idx = 0;
        //-- auto ref
        for (auto& i : Derived::scv) {
            __android_log_print(ANDROID_LOG_INFO, TAG, "by auto ref : %d, %.2x, %.2x", i, &i, &Derived::scv[idx]);
            idx++;
        }
    }

    void testEnum() {
        int a = 1;

        if (a == (int) DistortionType::zoom_in) {
            __android_log_print(ANDROID_LOG_INFO, TAG, "enum hit : %d", DistortionType::zoom_in);
        }

        __android_log_print(ANDROID_LOG_INFO, TAG, "enum DistortionType : %d", DistortionType::direction);

        Derived d;

        /*if (d == nullptr) {
            d = Derived(1, "bebop", 5.0);
        }*/

        d.debug();

    }

}



extern "C" {

using namespace Test;

JNIEXPORT void JNICALL
Java_com_example_likebebop_jnitest_JniTest_testAll(JNIEnv *env, jobject instance) {

    testString();
    testStruct();
    testClass();
    testSharedPtr();
    testLoop();
    testLamda();

    testUniquePtr();

    testEnum();

}


};