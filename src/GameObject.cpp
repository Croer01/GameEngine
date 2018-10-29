//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include <sstream>
#include "GameObject.hpp"
#include "Component.hpp"
#include "ObjectManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <yaml-cpp/yaml.h>

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
    for (auto &component : components_) {
        component->init();
    }
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

    cloned->position_ = position_;
    cloned->rotation_ = rotation_;
    cloned->scale_ = scale_;

    for (auto &component : components_) {
        std::shared_ptr<Component> clonedComponent = component->Clone();
        cloned->addComponent(clonedComponent);
    }

    return cloned;
}

void GameObject::fromFile(const std::string &filename) {
    try {
        YAML::Node gameObjectConfig = YAML::LoadFile(filename);

        //object properties
        if(gameObjectConfig["position"]){
            YAML::Node node = gameObjectConfig["position"];
            if(!node.IsSequence() || node.size() != 3)
                throw std::runtime_error("position must be a sequence of 3 numeric values.");
            this->setPosition(glm::vec3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>()));
        }
        if(gameObjectConfig["rotation"]){
            YAML::Node node = gameObjectConfig["rotation"];
            if(!node.IsSequence() || node.size() != 3)
                throw std::runtime_error("rotation must be a sequence of 3 numeric values.");
            this->setRotation(glm::vec3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>()));
        }
        if(gameObjectConfig["scale"]){
            YAML::Node node = gameObjectConfig["scale"];
            if(!node.IsSequence() || node.size() != 3)
                throw std::runtime_error("scale must be a sequence of 3 numeric values.");
            this->setScale(glm::vec3(node[0].as<double>(), node[1].as<double>(), node[2].as<double>()));
        }

        //load components
        YAML::Node components = gameObjectConfig["components"];
        for (auto i = 0; i < components.size(); ++i) {
            YAML::Node componentConfig = components[i];
            std::shared_ptr<Component> component = ObjectManager::GetInstance().createComponent(
                    componentConfig["type"].as<std::string>());
            component->fromFile(componentConfig);
            addComponent(component);
        }

    } catch (const std::exception &e) {
        throw std::runtime_error("Can't load '" + filename + "'. cause: " + e.what());
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
    return glm::translate(glm::mat4(1),position_) * glm::mat4_cast(glm::quat(rotation_)) * glm::scale(glm::mat4(1),scale_);
}
