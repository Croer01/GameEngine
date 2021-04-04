//
// Created by adria on 11/07/2020.
//

#ifndef GAMEENGINE_SPAWNERCOMPONENT_HPP
#define GAMEENGINE_SPAWNERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/components/ComponentData.hpp>
#include <game-engine/geGameObject.hpp>

namespace GameEngine
{

class SpawnerComponentData : public ComponentData
{
public:
    SpawnerComponentData()
    {
        createProperty<std::string>("targetName", "", true);
        createProperty<int>("startSize", 0, true);
    }
};

class SpawnerComponent : public geComponentInstantiable<SpawnerComponent,SpawnerComponentData>
{
    std::vector<geGameObjectRef> pool_;
    std::string targetName_;
    int startSize_;
public:
    void init() override;

    geGameObjectRef getNext() const;
};
}

#endif //GAMEENGINE_SPAWNERCOMPONENT_HPP
