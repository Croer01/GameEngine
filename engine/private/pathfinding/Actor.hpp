//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_ACTOR_HPP
#define GAMEENGINE_ACTOR_HPP


#include <game-engine/api.hpp>
#include <game-engine/events/Subject.hpp>

namespace GameEngine {
namespace Internal {

enum class ActorEvents{
    MOVING
};

class Actor : Subject<ActorEvents>
{
    Vec2D position_;
    void setPosition(const Vec2D &position);
};
}
}


#endif //GAMEENGINE_ACTOR_HPP
