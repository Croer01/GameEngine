//
// Created by adria on 08/04/2020.
//

#include <game-engine/geEnvironment.hpp>
#include <game-engine/internal/Environment.hpp>

namespace GameEngine
{

geEnvironment::~geEnvironment() {}

geEnvironmentRef geEnvironment::createInstance()
{
    return std::make_shared<Internal::Environment>();
}

}
