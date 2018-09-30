//
// Created by adria on 25/09/2018.
//

#include "ObjectManager.hpp"

void ObjectManager::registerComponentBuilder(const std::string &idType, ComponentBuilder *builder) {
    componentFactory_.AddBuilder(idType,builder);
}

std::shared_ptr<Component> ObjectManager::createComponent(const std::string &idType) {
    return componentFactory_.Create(idType);
}
