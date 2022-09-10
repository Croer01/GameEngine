//
// Created by adria on 01/11/2018.
//
#ifndef GAMEENGINE_OBSERVER_HPP
#define GAMEENGINE_OBSERVER_HPP

#include <game-engine/api.hpp>

namespace GameEngine {
template <typename... ArgsT>
class Subject;

template <typename... ArgsT>
class Observer
{
public:
    virtual ~Observer()
    {}

    virtual void onEvent(const Subject<ArgsT...> &target, ArgsT ...args) = 0;
};
}

#endif //GAMEENGINE_OBSERVER_HPP
