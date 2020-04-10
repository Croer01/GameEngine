//
// Created by adria on 08/04/2020.
//

#include <game-engine/geGame.hpp>
#include "../private/Game.hpp"

namespace GameEngine
{

geGame::~geGame() {}

geGameRef geGame::createInstance(const geEnvironmentRef &env)
{
    auto envInternal = std::dynamic_pointer_cast<Internal::Environment>(env);
    return std::make_shared<Internal::Game>(envInternal);
}

}
