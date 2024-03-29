//
// Created by adria on 25/09/2018.
//

#include <game-engine/internal/ObjectManager.hpp>
#include <game-engine/internal/GameObject.hpp>
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
        static std::string extension = ".prototype";
        if(filename.substr(filename.length() - extension.length()) != extension)
            throw std::invalid_argument("filename must have " + extension + " extension");

        auto it = prototypes_.find(objectType);

        if (it != prototypes_.end())
            throw std::runtime_error(("prototype " + objectType + " already registered").c_str());

        GameObject prototype(objectType);
        prototype.fromFile(filename, this);
        prototypes_[objectType] = std::move(prototype);
    }

    std::shared_ptr<GameObject> ObjectManager::createGameObject(const std::string &objectType, Game *game) {
        auto it = prototypes_.find(objectType);

        if (it == prototypes_.end())
            throw std::runtime_error(("prototype " + objectType + " not found").c_str());


        return it->second.Clone(game);
    }

    ObjectManager::~ObjectManager() {
        prototypes_.clear();
        componentFactory_ = Factory<geComponent, ComponentBuilder, std::string>();
    }

std::vector<std::string> ObjectManager::getComponentIds() const
{
    return componentFactory_.getIds();
}

ComponentDataRef ObjectManager::createProperties(const std::string &idType)
{
    return componentFactory_.createProperties(idType);
}
}
}