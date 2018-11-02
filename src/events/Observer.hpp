//
// Created by adria on 01/11/2018.
//
#ifndef SPACEINVADERS_OBSERVER_HPP
#define SPACEINVADERS_OBSERVER_HPP

template <typename EventType>
class Subject;

template <typename EventType>
class Observer {
public:
    virtual void onEvent(const Subject<EventType> &target, const EventType &event, void *args) = 0;
};


#endif //SPACEINVADERS_OBSERVER_HPP