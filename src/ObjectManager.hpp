//
// Created by adria on 25/09/2018.
//

#ifndef SPACEINVADERS_OBJECTMANAGER_HPP
#define SPACEINVADERS_OBJECTMANAGER_HPP


#include "Singleton.hpp"
#include "Factory.hpp"
#include "Component.hpp"
#include "Builder.hpp"

//declarations of the global methods generated to use in the ObjectManager
void RegisterComponents();

class ObjectManager : public Singleton<ObjectManager> {
private:
    Factory<Component,ComponentBuilder,std::string> componentFactory_;
public:
    void registerComponentBuilder(const std::string &idType, ComponentBuilder* builder);
    Component *createComponent(const std::string &idType);
};


#endif //SPACEINVADERS_OBJECTMANAGER_HPP
