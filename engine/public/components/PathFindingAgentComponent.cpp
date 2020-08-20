//
// Created by adria on 27/04/2020.
//

#include <game-engine/components/PathFindingAgentComponent.hpp>
#include <memory>
#include "../../private/pathfinding/AStarPathfinding.hpp"
#include "../../private/Game.hpp"

namespace GameEngine
{

void PathFindingAgentComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args)
{
    if(event == GameObjectEvent::PositionChanged)
    {
        agent_->setPosition(gameObject()->position());
    }
    else if(event == GameObjectEvent::ActiveChanged)
    {
        if(gameObject()->active() && active_)
            registerAgent();
        else
            unregisterAgent();
    }
}

PropertySetBase *PathFindingAgentComponent::getProperties() const
{
    auto *propertySet = new PropertySet<PathFindingAgentComponent>();

    propertySet->add(new Property<PathFindingAgentComponent, bool>(
        "active",
        &PathFindingAgentComponent::active,
        &PathFindingAgentComponent::active,
        true
    ));

    return propertySet;
}

PathFindingAgentComponent::~PathFindingAgentComponent()
{
    unregisterAgent();
}

void PathFindingAgentComponent::init()
{
    registerAgent();
}

void PathFindingAgentComponent::registerAgent()
{
    Internal::AStarPathfinding *pathfinder = dynamic_cast<Internal::Game*>(gameObject()->game())->pathFinder();
    if(pathfinder == nullptr)
        throw std::runtime_error("Current scene doesn't have a pathfinding configuration");

    agent_ = std::make_shared<Internal::Agent>();
    pathfinder->addAgent(agent_);
}

void PathFindingAgentComponent::unregisterAgent()
{
    if(agent_)
    {
        Internal::AStarPathfinding *pathfinder = dynamic_cast<Internal::Game*>(gameObject()->game())->pathFinder();
        if(pathfinder != nullptr)
            pathfinder->removeAgent(agent_);
    }
}

void PathFindingAgentComponent::active(const bool &value)
{
    active_ = value;

    if(gameObject()->active() && active_)
        registerAgent();
    else
        unregisterAgent();
}

bool PathFindingAgentComponent::active() const
{
    return active_;
}

}
