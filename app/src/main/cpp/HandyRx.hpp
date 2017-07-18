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
        BehaviorSubject<T>& owner;
        std::shared_ptr<std::function<void(int&)>> observer;
    public:
        Subscription(BehaviorSubject<T>& owner, std::shared_ptr<std::function<void(int&)>> observer) : owner(owner), observer(observer){
        }

        void unsubscribe() {
            owner.unsubscribe(observer);
        }
    };

    template <class T>
    class BehaviorSubject {
    private:
        std::vector<std::shared_ptr<std::function<void(int&)>>> observers;
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

        inline Subscription<T> subscribe(std::function<void(T&)> observer) {
            std::shared_ptr<std::function<void(int&)>> o = std::shared_ptr<std::function<void(int&)>>(new std::function<void(int&)>(observer));
            observers.push_back(o);
            return Subscription<T>(*this, o);
        }

        inline void unsubscribe(std::shared_ptr<std::function<void(int&)>> observer) {
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
