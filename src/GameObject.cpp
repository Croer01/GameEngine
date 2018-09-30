//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include <sstream>
#include "GameObject.hpp"
#include "Component.hpp"
#include "ObjectManager.hpp"
#include <INIReader.h>

int GameObject::ID_GENERATOR = 0;

GameObject::GameObject(std::string name) :
        name_(name),
        id_(ID_GENERATOR++) {
}

void GameObject::Init() {
    std::cout << "Object " << name_.c_str() << " initialized" << std::endl;
}

void GameObject::Update(float elapsedTime){
    for (auto &component : components_) {
        component->Update(elapsedTime);
    }
}

void GameObject::addComponent(std::shared_ptr<Component> component) {
    auto it = std::find(components_.begin(),components_.end(),component);
    if(it != components_.end())
        return;
    component->SetParent(this);
    components_.push_back(component);
}

GameObject::~GameObject() {
    for (auto &component : components_) {
        component->SetParent(nullptr);
    }
    components_.clear();
}

std::shared_ptr<GameObject> GameObject::Clone() const {
    auto cloned = std::make_shared<GameObject>(name_);
    //TODO: register clone to the ObjectManager

    for (auto &component : components_) {
        std::shared_ptr<Component> clonedComponent = component->Clone();
        cloned->addComponent(clonedComponent);
        //TODO: register clone to the ObjectManager
    }

    return cloned;
}

void GameObject::fromFile(const std::string &filename) {
    INIReader reader(filename);
    if (reader.ParseError() < 0) {
        throw std::runtime_error("Can't load '" + filename + "'");
    }

    name_ = reader.Get("","name","");
    std::string components = reader.Get("","components","");

    std::stringstream ss(components);
    std::string componentType;
    while(std::getline(ss, componentType, ' '))
    {
        std::shared_ptr<Component> component = ObjectManager::GetInstance().createComponent(componentType);
        component->fromFile(&reader);
        addComponent(component);
    }
}
