//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_ACTOR_HPP
#define GAMEENGINE_ACTOR_HPP


#include <memory>
#include <game-engine/api.hpp>
#include <game-engine/events/Subject.hpp>

namespace GameEngine {
namespace Internal {

enum class AgentEvents{
    MOVING
};

class Agent;
typedef std::shared_ptr<Agent> AgentRef;

class Agent : public Subject<AgentEvents>
{
    Vec2D position_;
public:
    void setPosition(const Vec2D &position);
    Vec2D getPosition() const;
};
}
}


#endif //GAMEENGINE_ACTOR_HPP
