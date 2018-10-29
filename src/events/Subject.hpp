//
// Created by adria on 14/10/2018.
//

#ifndef SPACEINVADERS_SUBJECT_HPP
#define SPACEINVADERS_SUBJECT_HPP

#include <utility>
#include <map>
#include <vector>
#include <functional>

template <typename EventType>
class Subject {

    std::map<EventType, std::vector<std::function<void()>>> observers_;
public:
    Subject()=default;
    // disallow copying and assigning
    Subject(const Subject&)=delete;
    Subject& operator=(const Subject&)=delete;

    template <typename Observer>
    void registerObserver(const EventType& event, Observer&& observer)
    {
        observers_[event].push_back(std::forward<Observer>(observer));
    }

    void notify(const EventType& event) const
    {
        auto it = observers_.find(event);
        if(it != observers_.end()) {
            for (const auto &observer : it->second) {
                observer();
            }
        }
    }
};


#endif //SPACEINVADERS_SUBJECT_HPP
