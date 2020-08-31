//
// Created by adria on 29/08/2020.
//

#ifndef GAMEENGINE_MACHINESTATE_HPP
#define GAMEENGINE_MACHINESTATE_HPP

namespace GameEngine {

template<typename T>
class FiniteStateMachine;

template<typename T>
class MachineState
{
public:
    virtual void enter(FiniteStateMachine<T> *fsm) = 0;
    virtual void update(FiniteStateMachine<T> *fsm, float elapsedTime) = 0;
    virtual void exit(FiniteStateMachine<T> *fsm) = 0;

    virtual ~MachineState()
    {}
};

template<typename T>
class FiniteStateMachine
{
    T *target_;
    MachineState<T> *currentState_;
public:
    FiniteStateMachine(T *target, MachineState<T> *newState)
        : target_(target), currentState_(newState)
    {
        currentState_->enter(this);
    }

    void changeState(MachineState<T> &newState)
    {
        currentState_->exit(this);
        currentState_ = &newState;
        currentState_->enter(this);
    };

    void update(float elapsedTime)
    {
        currentState_->update(this, elapsedTime);
    }

    T *getTarget()
    {
        return target_;
    }
};
}

#endif //GAMEENGINE_MACHINESTATE_HPP
