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
        std::function<void(T&)> observer;
    public:
        Subscription(BehaviorSubject<T>& owner, std::function<void(T&)> observer) : owner(owner), observer(observer){
        }

        void unsubscribe() {
            owner.unsubscribe(observer);
        }
    };

    template <class T>
    class BehaviorSubject {
    private:
        std::vector<std::function<void(T&)>> observers;
        T value;
        bool distinct = false;
        void notifyChanged() {
            for (auto& f : observers) {
                f(value);
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
            observers.push_back(observer);
            return Subscription<T>(*this, observer);
        }

        inline void unsubscribe(std::function<void(T&)> observer) {
            //const std::vector<std::function<void(T &)>>::iterator &p = std::find(observers.begin(), observers.end(), observer);
//            int idx = 0;
//            for (auto o : observers) {
//                if (o == observer) {
//                    observers.erase(observers.begin() + idx);
//                    break;
//                }
//                idx++;
//            }
            //observers.begin();
           // std::find(observers.begin(), observers.end(), observer);
//            if (p != observers.end()) {
//                observers.erase(p);
//            }
            //observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
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
