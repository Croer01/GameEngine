//
// Created by adria on 22/04/2020.
//

#include "Agent.hpp"

void GameEngine::Internal::Agent::setPosition(const GameEngine::Vec2D &position)
{
if(position_ != position)
{
    Vec2D prevPosition = position_;
    position_ = position;
    notify(AgentEvents::MOVING, &prevPosition);
}
}

GameEngine::Vec2D GameEngine::Internal::Agent::getPosition() const
{
    return position_;
}
