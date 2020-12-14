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

PropertySetBase *SpawnerComponent::getProperties() const
{
    auto *properties = new PropertySet<SpawnerComponent>();

    properties->add(new Property<SpawnerComponent, std::string>(
        "targetName",
        &SpawnerComponent::targetName,
        &SpawnerComponent::targetName,
        "",
        true));

    properties->add(new Property<SpawnerComponent, int>(
        "startSize",
        &SpawnerComponent::startSize,
        &SpawnerComponent::startSize,
        0,
        true));

    return properties;
}

std::string SpawnerComponent::targetName() const
{
    return targetName_;
}

void SpawnerComponent::targetName(const std::string &name)
{
    targetName_ = name;
}

int SpawnerComponent::startSize() const
{
    return startSize_;
}

void SpawnerComponent::startSize(const int &size)
{
    startSize_ = size;
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
