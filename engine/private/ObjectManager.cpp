//
// Created by adria on 25/09/2018.
//

#include "ObjectManager.hpp"
#include <exception>
#include <memory>

namespace GameEngine {
namespace Internal {
    void ObjectManager::registerComponentBuilder(const std::string &idType, ComponentBuilder *builder) {
        componentFactory_.AddBuilder(idType, builder);
    }

    geComponentRef ObjectManager::createComponent(const std::string &idType) {
        try {
            return componentFactory_.Create(idType);
        }
        catch (const std::exception &e) {
            throw std::runtime_error("error instantiating component " + idType + ". cause: " + e.what());
        }
    }
    geComponentRef ObjectManager::createComponent(const std::string &idType, const YAML::Node &data) {
        try {
            return componentFactory_.Create(idType, Data(data));
        }
        catch (const std::exception &e) {
            throw std::runtime_error("error instantiating component " + idType + ". cause: " + e.what());
        }
    }

    void ObjectManager::registerPrototype(const std::string &objectType, const std::string &filename) {
        auto it = prototypes_.find(objectType);

        if (it != prototypes_.end())
            throw std::runtime_error(("prototype " + objectType + " already registered").c_str());

        std::unique_ptr<GameObject> prototype;
        prototype.reset(new GameObject(objectType));
        prototype->fromFile(filename, this);
        prototypes_.insert(std::make_pair(objectType, std::move(prototype)));
    }

    std::shared_ptr<GameObject> ObjectManager::createGameObject(const std::string &objectType, Game *game) {
        auto it = prototypes_.find(objectType);

        if (it == prototypes_.end())
            throw std::runtime_error(("prototype " + objectType + " not found").c_str());


        return it->second->Clone(game);
    }

    ObjectManager::~ObjectManager() {
        prototypes_.clear();
        componentFactory_ = Factory<geComponent, ComponentBuilder, std::string>();
    }

std::vector<std::string> ObjectManager::getComponentIds() const
{
    return componentFactory_.getIds();
}

std::shared_ptr<PropertySetBase> ObjectManager::createProperties(const std::string &idType)
{
    return componentFactory_.createProperties(idType);
}
}
}