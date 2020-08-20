//
// Created by adria on 27/04/2020.
//

#ifndef GAMEENGINE_PATHFINDINGAGENTCOMPONENT_HPP
#define GAMEENGINE_PATHFINDINGAGENTCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include "../../../private/pathfinding/Agent.hpp"

namespace GameEngine
{

COMPONENT(PathFindingAgentComponent)
class PathFindingAgentComponent : public geComponentInstantiable<PathFindingAgentComponent>, public Observer<GameObjectEvent>
{
    Internal::AgentRef agent_;
    bool active_;
public:
    virtual void init();
    virtual ~PathFindingAgentComponent();
    virtual void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args);
    virtual PropertySetBase *getProperties() const;

    bool active() const;
    void active(const bool &value);
private:
    void registerAgent();
    void unregisterAgent();
};

}


#endif //GAMEENGINE_PATHFINDINGAGENTCOMPONENT_HPP
