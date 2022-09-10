//
// Created by adria on 22/04/2020.
//

#include <game-engine/pathfinding/Agent.hpp>

namespace GameEngine
{

void Agent::setPosition(const GameEngine::Vec2D &position)
{
    if(position_ != position)
    {
        Vec2D prevPosition = position_;
        position_ = position;
        notify(AgentEvents::MOVING, prevPosition);
    }
}

GameEngine::Vec2D Agent::getPosition() const
{
    return position_;
}

}