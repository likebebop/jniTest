#include <jni.h>
#include <string>
#include <android/log.h>
#include <vector>
#include <map>
#include "HandyRx.hpp"

using namespace std;




using namespace HandyRx;



namespace My {
    static const char* TAG = "likebebop";
    void debug(const char *s) {
        __android_log_print(ANDROID_LOG_INFO, TAG, s, "");
    }
}



namespace My {
    //-- 선언은 에러번 올수 있다.
    class Base;

    class Base;

    //-- scope enum
    enum class DistortionType {
        none = 0, zoom_in, zoom_out, direction, ellipseZoomin, ellipseZoomout, ellipseDirection, uniformZoomInWithoutAdjustment, uniformZoomInWithAdjustment, uniformZoomOut
    };

    struct BaseStruct {
        int id;

    };

    /*void onValue2(int& v) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "=== onValue2 %d", v);
    }*/

    class Student {
    public:
        //-- static const init은 에러 발생시킨다, 대신 constexpr사용해야되는듯;
        //static const float t = 1.0;
        //static constexpr float test = 1.0;
        int id;
        string name;
        float percentage;
        BehaviorSubject<int> myBehavior = BehaviorSubject<int>(5).distinctUntilChanged();

        shared_ptr<Subscription<int>> mySub = nullptr;

        Student() {

        }

        string& getName() {
            return name;
        }

        inline string& getNameByInline() {
            return name;
        }

        void onV(int& v) {
            __android_log_print(ANDROID_LOG_INFO, TAG, "=== onV %d, %s", v, name.c_str());
        }

        void debug() {
            char buf[1024];
            sprintf(buf, "id : %d, name : %s, p : %.2f, addr of getName : %0x, addr of getNameByInline : %0x", id, name.c_str(), percentage, &(getName()), &(getNameByInline()));
            My::debug(buf);
            sprintf(buf, "== id : %d, name : %s, p : %.2f, addr of getName : %0x, addr of getNameByInline : %0x", id, name.c_str(), percentage, &(getName()), &(getNameByInline()));
            My::debug(buf);
//            BehaviorSubject<int> s = BehaviorSubject<int>(1).distinctUntilChanged();
//            s.subscribe([&](int& v) {
//                __android_log_print(ANDROID_LOG_INFO, TAG, "=== onValue3 %d, %s", v, this->name.c_str());
//                this->onV(v);
//            });
//            //s.subscribe(onValue);
//            s.onNext(3);
//            s.onNext(3);
//            s.onNext(5);
//            s.onNext(3);
        }

        void observe() {
            mySub = myBehavior.subscribeShared([&](int&v) {
                __android_log_print(ANDROID_LOG_INFO, TAG, "on myBehavior : %d", v);
                this->onV(v);
            });
        }
    };

    class Base {

    public :
        Base(int id) {
            this->id = id;
            _name = __func__;
        }
        const char* _name;
        int id;
        virtual const char* className() const { return typeid(*this).name(); }
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

        virtual const char* className() const override  { return typeid(*this).name(); }

        ~Derived() {
            My::debug("(-)Derived");
        }
    };

    class Derived2 : public Derived {

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

    void onValue(int& v) {
        __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== onValue %d", v);
    }

    //template <class T>
    //typedef std::shared_ptr<std::function<void(T&)>> functionPointer;

    void testHandyRx() {
        BehaviorSubject<int> s = BehaviorSubject<int>(1).distinctUntilChanged();
        //BehaviorSubject<int> s = BehaviorSubject<int>(1);
        //onValue;

        std::shared_ptr<std::function<void(int&)>> f = std::shared_ptr<std::function<void(int&)>>(new std::function<void(int&)>(onValue));

        std::shared_ptr<std::function<void(int&)>> f2 = std::shared_ptr<std::function<void(int&)>>(new std::function<void(int&)>(onValue));

//        if (f == f2) {
//            __android_log_print(ANDROID_LOG_INFO, TAG, "=== pointer equals ===");
//            return;
//        }

//        std::function<void(int&)> f3 = onValue;
//        std::function<void(int&)>& f4 = f3;
//
//        //std::shared_ptr<std::function<void(int&)>> f2 = new std::function<void(int&)>(onValue);
//
//        if (f4 == f4) {
//            __android_log_print(ANDROID_LOG_INFO, TAG, "=== pointer equals ===");
//        }

        Subscription<int> sub1 = s.subscribe(onValue);
        Subscription<int> sub2 = s.subscribe(onValue);
        s.onNext(3);
        s.onNext(3);
        s.onNext(5);
        s.onNext(3);

        sub1.unsubscribe();
        s.onNext(5);
    }

    class TestClass {
    public:
        std::shared_ptr<Subscription<int>> intSub = nullptr;

        PublishSubject<int> s;
        void init() {
            intSub = s.subscribeShared([&](int& a) {
                __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== TestClass s %d", a);
            });
        }
    };

    void testPublishSubject2() {
        __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== begin of test");
        TestClass* t= new TestClass();
        t->init();
        delete t;
        __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== end of test");
    }

    void testPublishSubject() {
        PublishSubject<int> s = PublishSubject<int>();

//        std::shared_ptr<std::function<void(int&)>> f = std::shared_ptr<std::function<void(int&)>>(new std::function<void(int&)>(onValue));

//        std::shared_ptr<std::function<void(int&)>> f2 = std::shared_ptr<std::function<void(int&)>>(new std::function<void(int&)>(onValue));

        s.onNext(1);
//        FuncPt<int> ptr = [&](int& a) {
//
//        });
        FuncPt<int> ptr = [&](int& a) {
            __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== my inline function %d", a);
        };
        //Subscription<int> sub1 = s.subscribe(ptr);
        std::shared_ptr<Subscription<int>> sub2 = s.subscribeShared(onValue);
        s.onNext(3);
        s.onNext(3);
        s.onNext(5);
        s.onNext(3);
       // sub1.unsubscribe();
        s.onNext(5);


    }


    class Resource {
    public:
        Resource() {
        }
        Resource(int length, vector<unsigned char> c) : originalLength(length), compressed(c) {
        }

        ~Resource() {
        }

        vector<unsigned char> compressed;
        int originalLength;
        bool uncompressed = false;
    };

    map<std::string, Resource> resourceMap;

    static string TEST_KEY = "./res/shaders/passthrough.frag";

    string cstrToString(const char *str) {
        return (str == nullptr) ?  string() : string(str);
    }

    extern unsigned char passthrough_frag[];
    void testString() {
        My::debug("==== 웁스 =====");
        string s = "aa";
        debug(s.append(" bb"));


        string a = cstrToString(NULL);
        My::debug(a);
        My::debug("");

        a = cstrToString("ha");
        debug(a);
    }

    enum CallbackType : int {
        INIT,
        FINAL,
        FRAME_READY,
        RESET,
        BUFFER_SIZE_CHANGED,
        ASPECT_RATIO_CHANGED,
    };

    static const vector<const char*> callbackNames = {
        "initialize",
        "finalize",
        "frameReady",
        "reset",
        "onBufferSizeChanged",
        "onAspectRatioChanged",
    };

    class LuaCallback {
    public:
        bool hasCallback = false;
        const char* name = nullptr;
        LuaCallback(const char* name) : name(name) {
        }
        ~LuaCallback() {
            __android_log_print(ANDROID_LOG_INFO, My::TAG, "~LuaCallback  (%s)", name);
        }
        void debug() {
            __android_log_print(ANDROID_LOG_INFO, My::TAG, "=== LuaCallback  (%s)", name);
        }
    };

    void testVector() {
        map<CallbackType, shared_ptr<LuaCallback>> callMap;
        int i = 0;
        for (auto& name : callbackNames) {
            callMap[(CallbackType)i] = shared_ptr<LuaCallback>(new LuaCallback(name));
            i++;
        }

        for (auto& call : callMap) {
            call.second->debug();
        }
    }

    void testStruct() {
        Student s;
        s.name = "test";
        s.debug();

        Student s2;
        __android_log_print(ANDROID_LOG_INFO, TAG, "s %.2x, s2 %.2x", &s, &s2);
        __android_log_print(ANDROID_LOG_INFO, TAG, "after copy, s %.2x, s2 %.2x, %s", &s, &s2, s2.name.c_str());

        s2 = s;
        __android_log_print(ANDROID_LOG_INFO, TAG, "after copy, s %.2x, s2 %.2x, %s", &s, &s2, s2.name.c_str());

        char buf[1024];
        sprintf(buf, "* id : %d, name : %s, p : %.2f, addr of getName : %0x, addr of getNameByInline : %0x", s.id, s.name.c_str(), s.percentage, &(s.getName()), &(s.getNameByInline()));
        My::debug(buf);
        s.observe();
        s.myBehavior.onNext(10);
        s.myBehavior.onNext(11);
        s.observe();
        s.myBehavior.onNext(12);

        PublishSubject<int> test = PublishSubject<int>();
        test.onNext(1);
    }

    void testClass() {
        Derived s = {1, "bebop", 0.5};
        Derived s2 = s;
        Derived& ref = s;
        Base& base = ref;
        Derived2 derived2;
        Base* pb = &derived2;
        s.debug();
        s2.name = "test";
        s2.debug();
        ref.debug();
        ref.name = "hello bebop";
        s.debug();
        __android_log_print(ANDROID_LOG_INFO, TAG, "=== typeid test (%s, %s, %s)", typeid(ref).name(), ref._name, ref.className());

        __android_log_print(ANDROID_LOG_INFO, TAG, "=== typeid test2 (%s, %s, %s)", typeid(base).name(), base._name, base.className());
        __android_log_print(ANDROID_LOG_INFO, TAG, "=== typeid test3 (%s, %s, %s)", typeid(*pb).name(), pb->_name, pb->className());

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

    class Singleton {
    private:
        Singleton();
        /*Singleton(const Singleton& other) {};
        ~Singleton() {};*/
    public:
        int count;
        //static Singleton _instance;
        inline static Singleton& instance() {
            static Singleton instance;
            return instance;
        }

        inline void debug() {
            __android_log_print(ANDROID_LOG_INFO, TAG, "test : %d", count);
        }

    };

    Singleton::Singleton() {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Singleton created");
    }

    //Singleton Singleton::_instance = Singleton();



    void testSingleton() {
        Singleton::instance().count++;
        Singleton::instance().debug();
    }


    void testBinaryText() {
        //string frag((const allocator<char> &) passthrough_frag);
        //__android_log_print(ANDROID_LOG_INFO, TAG, (const char *) passthrough_frag, "");
       resourceMap[TEST_KEY] = Resource(100, vector<unsigned char> {
                0x23, 0x69, 0x66, 0x64, 0x65, 0x66, 0x20, 0x4f, 0x50, 0x45, 0x4e, 0x47,
                0x4c, 0x5f, 0x45, 0x53, 0x0a, 0x23, 0x69, 0x66, 0x64, 0x65, 0x66, 0x20,
                0x47, 0x4c, 0x5f, 0x46, 0x52, 0x41, 0x47, 0x4d, 0x45, 0x4e, 0x54, 0x5f,
                0x50, 0x52, 0x45, 0x43, 0x49, 0x53, 0x49, 0x4f, 0x4e, 0x5f, 0x48, 0x49,
                0x47, 0x48, 0x0a, 0x70, 0x72, 0x65, 0x63, 0x69, 0x73, 0x69, 0x6f, 0x6e,
                0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74,
                0x3b, 0x0a, 0x23, 0x65, 0x6c, 0x73, 0x65, 0x0a, 0x70, 0x72, 0x65, 0x63,
                0x69, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d,
                0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x3b, 0x0a, 0x23, 0x65, 0x6e,
                0x64, 0x69, 0x66, 0x0a, 0x23, 0x65, 0x6e, 0x64, 0x69, 0x66, 0x0a, 0x0a,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x0a,
                0x2f, 0x2f, 0x20, 0x55, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x73, 0x0a,
                0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70,
                0x6c, 0x65, 0x72, 0x32, 0x44, 0x20, 0x75, 0x5f, 0x74, 0x65, 0x78, 0x74,
                0x75, 0x72, 0x65, 0x3b, 0x0a, 0x0a, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f,
                0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x0a, 0x2f, 0x2f, 0x20, 0x56, 0x61, 0x72,
                0x79, 0x69, 0x6e, 0x67, 0x73, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e,
                0x67, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78,
                0x43, 0x6f, 0x6f, 0x72, 0x64, 0x3b, 0x0a, 0x0a, 0x0a, 0x76, 0x6f, 0x69,
                0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20,
                0x20, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x63, 0x6f, 0x6c, 0x6f,
                0x72, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x32,
                0x44, 0x28, 0x75, 0x5f, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x2c,
                0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x29,
                0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61,
                0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63,
                0x34, 0x28, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 0x72, 0x67, 0x62, 0x2c,
                0x20, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x2e, 0x61, 0x29, 0x3b, 0x0a, 0x20,
                0x20, 0x20, 0x20, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x2f, 0x69, 0x66,
                0x28, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6f, 0x72, 0x64, 0x2e,
                0x78, 0x20, 0x3e, 0x20, 0x30, 0x2e, 0x35, 0x29, 0x20, 0x67, 0x6c, 0x5f,
                0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20,
                0x76, 0x65, 0x63, 0x34, 0x28, 0x31, 0x2e, 0x30, 0x2c, 0x20, 0x30, 0x2c,
                0x20, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x0a, 0x7d,
                0x0a
        });

        __android_log_print(ANDROID_LOG_INFO, TAG, "testBinaryText => %d", resourceMap[TEST_KEY].compressed.size());
        __android_log_print(ANDROID_LOG_INFO, TAG, "testBinaryText 1 => %s", &(resourceMap[TEST_KEY].compressed)[0]);
    }

    void buildPtr(int **pInt) {
        if (*pInt == nullptr) {
            *pInt = new int(5);
        }
    }

    void testPointer() {
        int* pInt = nullptr;
        //__android_log_print(ANDROID_LOG_INFO, TAG, "pInt => %d", *pInt);
        buildPtr(&pInt);
        __android_log_print(ANDROID_LOG_INFO, TAG, "pInt => %d", *pInt);
        delete pInt;
    }

}



extern "C" {

using namespace Test;

JNIEXPORT void JNICALL
Java_com_example_likebebop_jnitest_JniTest_testAll(JNIEnv *env, jobject instance) {

    testPublishSubject2();
//    testPublishSubject();
//    testStruct();
//    testPointer();
    //testVector();
//
//    testSharedPtr();
//    testLoop();
//
//
//    testUniquePtr();
//
//    testEnum();
//    __android_log_print(ANDROID_LOG_INFO, TAG, "Singleton begin");
//    testSingleton();
//    testSingleton();
//    testSingleton();
//    testBinaryText();
//
//    __android_log_print(ANDROID_LOG_INFO, TAG, "testBinaryText => %s", &(resourceMap[TEST_KEY].compressed)[0]);
//
//    testLamda();
//    testClass();
//#ifdef _LOG
//    __android_log_print(ANDROID_LOG_INFO, TAG, "=== _LOG OK ===");
//#endif
//
//    testString();

}



};