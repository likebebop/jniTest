//
// Created by likebebop on 2017. 7. 17..
//

#ifndef HANDYRX_HPP
#define HANDYRX_HPP


#include <vector>
#include <functional>

namespace HandyRx {

    template <class T>
    class BehaviorSubject;

    static const char* TAG2 = "likebebop";

    template <class T>
    class Subscription {
    private:
        BehaviorSubject<T>* owner = nullptr;
        std::shared_ptr<std::function<void(T&)>> observer;
        bool unsubscribed = false;
    public:
        Subscription() : unsubscribed(true) {
            __android_log_print(ANDROID_LOG_INFO, TAG2, "===(+) Subscription %0x", this);
        }

        Subscription(BehaviorSubject<T>* owner, std::shared_ptr<std::function<void(T&)>> observer) : owner(owner), observer(observer){
            __android_log_print(ANDROID_LOG_INFO, TAG2, "===(+) Subscription2 %0x", this);
        }
        ~Subscription() {
            unsubscribe();
        }

        void unsubscribe() {
            __android_log_print(ANDROID_LOG_INFO, TAG2, "=== unsubscribe %0x", this);
            if (unsubscribed || !owner) {
                return;
            }
            owner->unsubscribe(observer);
            unsubscribed = true;
        }
    };

    template <class T>
    class BehaviorSubject {
    private:
        //-- https://stackoverflow.com/questions/4584685/vector-of-stdfunction
        //-- std::function이 operator==가 제대로 안됨 허허허..
        std::vector<std::shared_ptr<std::function<void(T&)>>> observers;
        T value;
        bool distinct = false;
        void notifyChanged() {
            for (auto& f : observers) {
                (*f.get())(value);
            }
        }
    public:
        BehaviorSubject(T t) : value(t) {
        };

        T& getValue() {
            return value;
        }

        BehaviorSubject& distinctUntilChanged() {
            distinct = true;
            return *this;
        }

        Subscription<T> subscribe(std::function<void(T&)> observer) {
            std::shared_ptr<std::function<void(T&)>> o = std::shared_ptr<std::function<void(T&)>>(new std::function<void(T&)>(observer));
            observers.push_back(o);
            return Subscription<T>(this, o);
        }

        std::shared_ptr<Subscription<T>> subscribeShared(std::function<void(T&)> observer) {
            return std::shared_ptr<Subscription<T>>(new Subscription<T>(subscribe(observer)));
        }

        void unsubscribe(std::shared_ptr<std::function<void(T&)>> observer) {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void onNext(T t) {
            bool changed = t != value;
            value = t;
            if (distinct && !changed) {
                return;
            }
            notifyChanged();
        }
    };

};


#endif //HANDYRX_HPP
