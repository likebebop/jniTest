//
// Created by likebebop on 2017. 7. 17..
//

#ifndef HANDYRX_HPP
#define HANDYRX_HPP


#include <vector>
#include <functional>
#include <android/log.h>

namespace HandyRx {
    static const char* TAG2 = "likebebop";


    template <class T>
    using FuncPt = std::function<void(T&)>;

    template <class T>
    class Unsubscribable {
    public:
        virtual void unsubscribe(std::shared_ptr<FuncPt<T>> observer) = 0;
    };

    template <class T>
    class Subscription {
    private:
        Unsubscribable<T>* owner = nullptr;
        std::shared_ptr<FuncPt<T>> observer;
        bool unsubscribed = false;
    public:
        Subscription() : unsubscribed(true) {
        }

        Subscription(Unsubscribable<T>* owner, std::shared_ptr<FuncPt<T>> observer) : owner(owner), observer(observer){
            __android_log_print(ANDROID_LOG_INFO, TAG2, "Subscription() %p", this);
        }
        ~Subscription() {
            unsubscribe();
            __android_log_print(ANDROID_LOG_INFO, TAG2, "~Subscription() %p", this);
        }

        void unsubscribe() {
            if (unsubscribed || !owner) {
                return;
            }
            owner->unsubscribe(observer);
            unsubscribed = true;
        }
    };

    template <class T>
    class BehaviorSubject : public Unsubscribable<T> {
    protected:
        //-- https://stackoverflow.com/questions/4584685/vector-of-stdfunction
        //-- std::function이 operator==가 제대로 안됨, 그래서 어쩔수 없이 shared_ptr 사용한다. 허허허..
        std::vector<std::shared_ptr<FuncPt<T>>> observers;
        T value;
        bool distinct = false;
        int _skipFirst = false;
        void notifyChanged() {
            for (auto& f : observers) {
                (*f.get())(value);
            }
        }
    public:
        BehaviorSubject(T t) : value(t) {
        };

        virtual T& getValue() {
            return value;
        }

        BehaviorSubject& distinctUntilChanged() {
            distinct = true;
            return *this;
        }

        BehaviorSubject& skipFirst() {
            _skipFirst = true;
            return *this;
        }
        Subscription<T> subscribe(FuncPt<T> observer) {
            std::shared_ptr<FuncPt<T>> o = std::shared_ptr<FuncPt<T>>(new FuncPt<T>(observer));
            observers.push_back(o);
            if (!_skipFirst) {
                notifyChanged();
            }
            return Subscription<T>(this, o);
        }

        std::shared_ptr<Subscription<T>> subscribeShared(FuncPt<T> observer) {
            return std::shared_ptr<Subscription<T>>(new Subscription<T>(subscribe(observer)));
        }

        void unsubscribe(std::shared_ptr<FuncPt<T>> observer) {
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

    template <class T>
    class PublishSubject : public Unsubscribable<T> {
    protected:
        //-- https://stackoverflow.com/questions/4584685/vector-of-stdfunction
        //-- std::function이 operator==가 제대로 안됨, 그래서 어쩔수 없이 shared_ptr 사용한다. 허허허..
        std::vector<std::shared_ptr<FuncPt<T>>> observers;
        T value;
        void notifyChanged() {
            for (auto& f : observers) {
                (*f.get())(value);
            }
        }
    public:
        Subscription<T> subscribe(FuncPt<T> observer) {
            std::shared_ptr<FuncPt<T>> o = std::shared_ptr<FuncPt<T>>(new FuncPt<T>(observer));
            observers.push_back(o);
            return Subscription<T>(this, o);
        }

        std::shared_ptr<Subscription<T>> subscribeShared(FuncPt<T> observer) {
            return std::shared_ptr<Subscription<T>>(new Subscription<T>(subscribe(observer)));
        }

        void unsubscribe(std::shared_ptr<FuncPt<T>> observer) {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void onNext(T t) {
            bool changed = t != value;
            value = t;
            notifyChanged();
        }
    };

};


#endif //HANDYRX_HPP
