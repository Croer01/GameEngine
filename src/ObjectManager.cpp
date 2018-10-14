//
// Created by adria on 25/09/2018.
//

#include "ObjectManager.hpp"
#include <exception>

void ObjectManager::registerComponentBuilder(const std::string &idType, ComponentBuilder *builder) {
    componentFactory_.AddBuilder(idType,builder);
}

std::shared_ptr<Component> ObjectManager::createComponent(const std::string &idType) {
    return componentFactory_.Create(idType);
}

void ObjectManager::registerPrototype(const std::string &objectType, const std::string &filename) {
    auto it = prototypes_.find(objectType);

    if(it != prototypes_.end())
        throw std::exception(("prototype " + objectType + " already registered").c_str());

    std::unique_ptr<GameObject> prototype = std::make_unique<GameObject>(objectType);
    prototype->fromFile(filename);
    prototypes_.insert(std::make_pair(objectType,std::move(prototype)));
}

std::shared_ptr<GameObject> ObjectManager::createGameObject(const std::string &objectType) {
    auto it = prototypes_.find(objectType);

    if(it == prototypes_.end())
        throw std::exception(("prototype " + objectType + " not found").c_str());


    return it->second->Clone();
}
