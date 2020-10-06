//
// Created by likebebop on 2017. 7. 17..
//

#ifndef HANDYRX_HPP
#define HANDYRX_HPP


#include <vector>
#include <functional>
#include <android/log.h>
#include <unordered_map>

namespace HandyRx {
    static const char* LTAG = "likebebop";


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
        bool unsubscribed = false;

    public:
        std::shared_ptr<FuncPt<T>> observer = nullptr;
        Subscription() : unsubscribed(true) {
        }

        Subscription(Unsubscribable<T>* owner, std::shared_ptr<FuncPt<T>> observer) : owner(owner), observer(observer){
            __android_log_print(ANDROID_LOG_INFO, LTAG, "[+]Subscription() %p, %p", this, owner);
        }
        ~Subscription() {
            unsubscribe();
            __android_log_print(ANDROID_LOG_INFO, LTAG, "[-]Subscription() %p, %p", this, owner);
        }

        void unsubscribe() {
            if (unsubscribed || !owner) {
                __android_log_print(ANDROID_LOG_INFO, LTAG, "[#]unsubscribe finished %p, %p", this, owner);
                return;
            }
            owner->unsubscribe(observer);
            unsubscribed = true;
            __android_log_print(ANDROID_LOG_INFO, LTAG, "[*]unsubscribe() %p, %p", this, owner);
        }
    };

    template <class T>
    class BehaviorSubject : public Unsubscribable<T> {
    protected:
        //-- https://stackoverflow.com/questions/4584685/vector-of-stdfunction
        //-- std::function이 operator==가 제대로 안됨, 그래서 어쩔수 없이 shared_ptr 사용한다. 허허허..
        std::unordered_map<std::shared_ptr<FuncPt<T>>, std::shared_ptr<Subscription<T>>> subscriptions;
        std::vector<std::shared_ptr<FuncPt<T>>> observers;

        T value;

        bool distinct = false;
        int _skipFirst = false;

        void notifyChanged() {
            for (auto& s : observers) {
                (*(s).get())(value);
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

        //-- clear subscriptions safely
        ~BehaviorSubject() {
            for (auto s : subscriptions) {
                s.second->unsubscribe();
            }
        }

        std::shared_ptr<Subscription<T>> subscribeShared(FuncPt<T> observer) {
            auto o = std::shared_ptr<FuncPt<T>>(new FuncPt<T>(observer));
            auto s = std::shared_ptr<Subscription<T>>(new Subscription<T>(this, o));
            subscriptions[o] = s;
            observers.push_back(o);
            if (!_skipFirst) {
                (*(o).get())(value);
            }
            return s;
        }

        void unsubscribe(std::shared_ptr<FuncPt<T>> observer) {
            auto it = subscriptions.find(observer);
            if (it == subscriptions.end()) {
                return;
            }
            __android_log_print(ANDROID_LOG_INFO, LTAG, "unsubscribe() begin %p, %d, %d", this, subscriptions.size(), observers.size());

            subscriptions.erase(it);
            observers.erase(std::remove(observers.begin(), observers.end(), observer),observers.end());
            __android_log_print(ANDROID_LOG_INFO, LTAG, "unsubscribe() end %p, %d, %d", this, subscriptions.size(), observers.size());

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
        //-- observer, subscription map
        std::unordered_map<std::shared_ptr<FuncPt<T>>, std::shared_ptr<Subscription<T>>> subscriptions;
        std::vector<std::shared_ptr<FuncPt<T>>> observers;

        T value;
        void notifyChanged() {
            for (auto& s : observers) {
                (*(s).get())(value);
            }
        }

    public:
        std::shared_ptr<Subscription<T>> subscribeShared(FuncPt<T> observer) {
            auto o = std::shared_ptr<FuncPt<T>>(new FuncPt<T>(observer));
            auto s = std::shared_ptr<Subscription<T>>(new Subscription<T>(this, o));
            subscriptions[o] = s;
            observers.push_back(o);
            return s;
        }

        void unsubscribe(std::shared_ptr<FuncPt<T>> observer) {
            auto it = subscriptions.find(observer);
            if (it == subscriptions.end()) {
                return;
            }
            __android_log_print(ANDROID_LOG_INFO, LTAG, "unsubscribe() begin %p, %d, %d", this, subscriptions.size(), observers.size());
            subscriptions.erase(it);
            observers.erase(std::remove(observers.begin(), observers.end(), observer),observers.end());

            __android_log_print(ANDROID_LOG_INFO, LTAG, "unsubscribe() end %p, %d, %d", this, subscriptions.size(), observers.size());
        }

        PublishSubject() {
            __android_log_print(ANDROID_LOG_INFO, LTAG, "[+]PublishSubject() %p", this);

        }

        //-- 순서가 문제 될수 있으므로 여기서 다 릴리즈 하면 좀더 안전
        ~PublishSubject() {
            for (auto s : subscriptions) {
                s.second->unsubscribe();
            }
            __android_log_print(ANDROID_LOG_INFO, LTAG, "[-]PublishSubject() %p", this);
        }

        void onNext(T t) {
            bool changed = t != value;
            value = t;
            notifyChanged();
        }
    };

};


#endif //HANDYRX_HPP
