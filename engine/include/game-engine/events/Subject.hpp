//
// Created by adria on 14/10/2018.
//

#ifndef GAMEENGINE_SUBJECT_HPP
#define GAMEENGINE_SUBJECT_HPP

#include <utility>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <game-engine/events/Observer.hpp>

namespace GameEngine {
    template <typename ...ArgsT>
    class Subject {
        typedef Observer<ArgsT...> ObserverType;

        std::vector<ObserverType *> observers_;
    public:
        Subject() = default;

        virtual ~Subject() {
            observers_.clear();
        }

        void registerObserver(ObserverType *observer) {
            observers_.push_back(observer);
        }

        void unregisterObserver(ObserverType *observer) {
            auto it = std::find(observers_.begin(), observers_.end(), observer);
            if (it != observers_.end())
                observers_.erase(it);
        }

        void notify(ArgsT ...args) const {
            for (auto observer : observers_) {
                observer->onEvent(*this, std::forward<ArgsT>(args)...);
            }
        }
    };
}

#endif //GAMEENGINE_SUBJECT_HPP
