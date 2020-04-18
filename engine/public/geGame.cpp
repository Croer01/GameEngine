//
// Created by adria on 08/04/2020.
//

#include <game-engine/geGame.hpp>
#include "../private/Game.hpp"

namespace GameEngine
{

geRendererLock::geRendererLock(std::mutex &mutex)
    : lock_(mutex),
    unlocked_(false)
{}

geRendererLock::geRendererLock(geRendererLock &&other) noexcept
    : lock_(std::move(other.lock_)),
      unlocked_(other.unlocked_)
{}

void geRendererLock::unlock()
{
    // in some cases the lock_ could be invalid because the object reference is moved
    if(lock_ && !unlocked_)
    {
        unlocked_ = true;
        lock_.unlock();
    }
}

geRendererLock::~geRendererLock()
{
    unlock();
}

geGame::~geGame() {}

geGameRef geGame::createInstance(const geEnvironmentRef &env)
{
    auto envInternal = std::dynamic_pointer_cast<Internal::Environment>(env);
    return std::make_shared<Internal::Game>(envInternal);
}

}
