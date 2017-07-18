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

    template <class T>
    class Subscription {
    private:
        BehaviorSubject<T>& owner;
        std::shared_ptr<std::function<void(T&)>> observer;
        bool unsubscribed = false;
    public:
        Subscription(BehaviorSubject<T>& owner, std::shared_ptr<std::function<void(T&)>> observer) : owner(owner), observer(observer){
        }
        ~Subscription() {
            unsubscribe();
        }

        void unsubscribe() {
            if (unsubscribed) {
                return;
            }
            owner.unsubscribe(observer);
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

        inline T& getValue() {
            return value;
        }

        inline BehaviorSubject& distinctUntilChanged() {
            distinct = true;
            return *this;
        }

        Subscription<T> subscribe(std::function<void(T&)> observer) {
            std::shared_ptr<std::function<void(T&)>> o = std::shared_ptr<std::function<void(T&)>>(new std::function<void(T&)>(observer));
            observers.push_back(o);
            return Subscription<T>(*this, o);
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
