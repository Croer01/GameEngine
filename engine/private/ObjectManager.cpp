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
        prototype->fromFile(filename);
        prototypes_.insert(std::make_pair(objectType, std::move(prototype)));
    }

    std::shared_ptr<GameObject> ObjectManager::createGameObject(const std::string &objectType) {
        auto it = prototypes_.find(objectType);

        if (it == prototypes_.end())
            throw std::runtime_error(("prototype " + objectType + " not found").c_str());


        return it->second->Clone();
    }

    void ObjectManager::clear() {
        prototypes_.clear();
        componentFactory_ = Factory<geComponent, ComponentBuilder, std::string>();
    }
}
}