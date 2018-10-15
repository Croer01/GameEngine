//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include <sstream>
#include "GameObject.hpp"
#include "Component.hpp"
#include "ObjectManager.hpp"
#include <INIReader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

int GameObject::ID_GENERATOR = 0;

GameObject::GameObject(const std::string &name) : Subject<GameObjectEvent>(),
        nameType_(name),
        id_(ID_GENERATOR++),
        position_(glm::vec3(0.f)),
        rotation_(glm::vec3(0.f)),
        scale_(glm::vec3(1.f)) {
}

void GameObject::Init() {
    std::cout << "Object " << nameType_.c_str() << " initialized" << std::endl;
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
    auto cloned = std::make_shared<GameObject>(nameType_);
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

    std::string components = reader.Get("","components","");

    std::stringstream ss(components);
    std::string componentType;
    while(std::getline(ss, componentType, ' '))
    {
        std::shared_ptr<Component> component = ObjectManager::GetInstance().createComponent(componentType);
        component->fromFile(reader);
        addComponent(component);
    }
}

const glm::vec3 &GameObject::getPosition() const {
    return position_;
}

void GameObject::setPosition(const glm::vec3 &position) {
    position_ = position;
    notify(GameObjectEvent::TransformChanged);
}

const glm::vec3 &GameObject::getRotation() const {
    return rotation_;
}

void GameObject::setRotation(const glm::vec3 &rotation) {
    rotation_ = rotation;
    notify(GameObjectEvent::TransformChanged);
}

const glm::vec3 &GameObject::getScale() const {
    return scale_;
}

void GameObject::setScale(const glm::vec3 &scale) {
    scale_ = scale;
    notify(GameObjectEvent::TransformChanged);
}

glm::mat4 GameObject::getTransform() {
    //remember the order of matrix multiplication is from right to left
    return glm::scale(glm::mat4(1),scale_) * glm::translate(glm::mat4(1),position_) * glm::mat4_cast(glm::quat(rotation_));
}
