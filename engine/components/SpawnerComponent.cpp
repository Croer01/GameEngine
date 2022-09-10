//
// Created by adria on 20/05/2019.
//

#include <game-engine/components/SpawnerComponent.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
void SpawnerComponent::init()
{
    for (int i = 0; i < startSize_; ++i)
    {
        geGameObjectRef go = gameObject()->game()->createFromPrototype(targetName_);
        go->active(false);
        pool_.push_back(go);
    }
}

geGameObjectRef SpawnerComponent::getNext() const
{
    geGameObjectRef instance;
    const auto &it = std::find_if(
        pool_.begin(),
        pool_.end(),
        [](const geGameObjectRef &go)
        {
            return !go->active();
        });

    if (it != pool_.end())
    {
        instance = *it;
        instance->active(true);
    }

    return instance;
}
}
