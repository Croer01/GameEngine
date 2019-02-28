//
// Created by adria on 14/10/2018.
//

#ifndef SPACEINVADERS_SUBJECT_HPP
#define SPACEINVADERS_SUBJECT_HPP

#include <utility>
#include <map>
#include <vector>
#include <functional>
#include <game-engine/events/Observer.hpp>

namespace GameEngine {
    template<typename EventType>
    class Subject {

        std::vector<Observer<EventType> *> observers_;
    public:
        Subject() = default;

        // disallow copying and assigning
        Subject(const Subject &) = delete;

        Subject &operator=(const Subject &) = delete;

        virtual ~Subject() {
            observers_.clear();
        }

        void registerObserver(Observer<EventType> *observer) {
            observers_.push_back(observer);
        }

        void unregisterObserver(Observer<EventType> *observer) {
            auto it = std::find(observers_.begin(), observers_.end(), observer);
            if (it != observers_.end()) {
                std::swap(*it, *(--observers_.end()));
                observers_.pop_back();
            }
        }

        void notify(const EventType &event, void *args) const {
            for (Observer<EventType> *observer : observers_) {
                observer->onEvent(*this, event, args);
            }
        }

        void notify(const EventType &event) const {
            for (Observer<EventType> *observer : observers_) {
                observer->onEvent(*this, event, nullptr);
            }
        }
    };
}

#endif //SPACEINVADERS_SUBJECT_HPP
