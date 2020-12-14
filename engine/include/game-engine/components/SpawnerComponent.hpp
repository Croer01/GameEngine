//
// Created by adria on 11/07/2020.
//

#ifndef GAMEENGINE_SPAWNERCOMPONENT_HPP
#define GAMEENGINE_SPAWNERCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>

namespace GameEngine
{
class SpawnerComponent : public geComponentInstantiable<SpawnerComponent>
{
    std::vector<geGameObjectRef> pool_;
    std::string targetName_;
    int startSize_;
public:
    void init() override;

    std::string targetName() const;
    void targetName(const std::string &name);

    int startSize() const;
    void startSize(const int &size);

    geGameObjectRef getNext() const;

    virtual PropertySetBase *getProperties() const;
};
}

#endif //GAMEENGINE_SPAWNERCOMPONENT_HPP
