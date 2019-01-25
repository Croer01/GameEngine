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


namespace GameEngine {
namespace Internal {
    int GameObject::ID_GENERATOR = 0;

    GameObject::GameObject(const std::string &name) : Subject<GameObjectEvent>(), Observer<GameObjectEvent>(),
            parent_(nullptr),
            nameType_(name),
            id_(ID_GENERATOR++),
            active_(true),
            activeValueToSetInSafeMode_(true),
            hasActiveValueToSetInSafeMode_(false),
            position_(0.f, 0.f),
            rotation_(0.f, 0.f),
            scale_(1.f,1.f) {
        computeTransform();
    }

    void GameObject::Init() {
        std::cout << "Object " << nameType_.c_str() << " initialized" << std::endl;
        for (auto &component : components_) {
            component->init();
        }
        for (auto &child : children_) {
            child->Init();
        }
    }

    void GameObject::Update(float elapsedTime){
        if(hasActiveValueToSetInSafeMode_){
            active_ = activeValueToSetInSafeMode_;
            hasActiveValueToSetInSafeMode_ = false;
            notify(GameObjectEvent::ActiveChanged);
        }

        if(!active_)
            return;
        for (auto &component : components_) {
            component->Update(elapsedTime);
        }

        for (auto &child : children_) {
            child->Update(elapsedTime);
        }
    }

    void GameObject::addComponent(std::shared_ptr<Component> component) {
        auto it = std::find(components_.begin(),components_.end(),component);
        if(it != components_.end())
            return;
        component->gameObject(this);
        components_.push_back(component);
    }

    void GameObject::addChild(std::shared_ptr<GameObject> child) {
        auto it = std::find(children_.begin(),children_.end(),child);
        if(it != children_.end())
            return;
        child->setParent(this);
        children_.push_back(child);
    }

    void GameObject::setParent(GameObject *parent) {
        if(parent_)
            parent_->unregisterObserver(this);
        parent_ = parent;
        parent_->registerObserver(this);
        notify(GameObjectEvent::TransformChanged);
    }

    GameObject::~GameObject() {
        for (auto &component : components_) {
            component->gameObject(nullptr);
        }
        components_.clear();
    }

    std::shared_ptr<GameObject> GameObject::Clone() const {
        auto cloned = std::make_shared<GameObject>(nameType_);

        cloned->name_ = name_;
        cloned->position_ = position_;
        cloned->rotation_ = rotation_;
        cloned->scale_ = scale_;

        for (auto &component : components_) {
            std::shared_ptr<Component> clonedComponent = component->Clone();
            if(!clonedComponent)
                throw std::runtime_error("one of the components of " + nameType_ + " has an error during cloning process");
            cloned->addComponent(clonedComponent);
        }

        for (auto &child : children_) {
            std::shared_ptr<GameObject> clonedChild = child->Clone();
            if(!clonedChild)
                throw std::runtime_error("one of the child of " + nameType_ + " has an error during cloning process");
            cloned->addChild(clonedChild);
        }

        return cloned;
    }

    void GameObject::fromFile(const std::string &filename) {
        try {
            YAML::Node gameObjectConfig = YAML::LoadFile(filename);
            fromYamlNode(gameObjectConfig);
        } catch (const std::exception &e) {
            throw std::runtime_error("Can't load '" + filename + "'. cause: " + e.what());
        }
    }

    Vec2D GameObject::position() const {
        Vec2D position = position_;
        if(parent_)
            position += parent_->position_;
        return position;
    }

    void GameObject::position(const Vec2D &position) {
        if(position_ != position) {
            position_ = position;
            notify(GameObjectEvent::PositionChanged);
            computeTransform();
            notify(GameObjectEvent::TransformChanged);
        }
    }

    Vec2D GameObject::rotation() const {
        Vec2D rotation = rotation_;
        if(parent_)
            rotation += parent_->rotation_;
        return rotation;
    }

    void GameObject::rotation(const Vec2D &rotation) {
        if(rotation_ != rotation) {
            rotation_ = rotation;
            notify(GameObjectEvent::RotationChanged);
            computeTransform();
            notify(GameObjectEvent::TransformChanged);
        }
    }

    Vec2D GameObject::scale() const {
        Vec2D scale = scale_;
        if(parent_)
            scale *= parent_->scale_;
        return scale;
    }

    void GameObject::scale(const Vec2D &scale) {
        if(scale_ != scale){
            scale_ = scale;
            notify(GameObjectEvent::ScaleChanged);
            computeTransform();
            notify(GameObjectEvent::TransformChanged);
        }
    }

    glm::mat4 GameObject::getTransform() {
        if(parent_)
            return parent_->getTransform() * transform_;

        return transform_;
    }

    bool GameObject::isActive() const {
        return activeValueToSetInSafeMode_;
    }

    void GameObject::setActive(bool active) {
        activeValueToSetInSafeMode_ = active;
        hasActiveValueToSetInSafeMode_ = true;
    }

    std::string GameObject::getName() const {
        return name_;
    }

    void GameObject::setName(const std::string &name) {
        name_ = name;
    }

    void GameObject::fromYamlNode(const YAML::Node &node) {
        if(node["name"])
            name_ = node["name"].as<std::string>();

        // TODO: refactor the position, the rotation and the scale to get arrays of length 2
        //object properties
        if(node["position"]){
            YAML::Node position = node["position"];
            if(!position.IsSequence() || position.size() != 3)
                throw std::runtime_error("position must be a sequence of 3 numeric values.");
            this->position(Vec2D(position[0].as<float>(), position[1].as<float>()));
        }
        if(node["rotation"]){
            YAML::Node rotation = node["rotation"];
            if(!rotation.IsSequence() || rotation.size() != 3)
                throw std::runtime_error("rotation must be a sequence of 3 numeric values.");
            this->rotation(Vec2D(rotation[0].as<float>(), rotation[1].as<float>()));
        }
        if(node["scale"]){
            YAML::Node scale = node["scale"];
            if(!scale.IsSequence() || scale.size() != 3)
                throw std::runtime_error("scale must be a sequence of 3 numeric values.");
            this->scale(Vec2D(scale[0].as<float>(), scale[1].as<float>()));
        }

        //load components
        YAML::Node components = node["components"];
        if(components) {
            for (auto i = 0; i < components.size(); ++i) {
                YAML::Node componentConfig = components[i];
                std::shared_ptr<Component> component = ObjectManager::GetInstance().createComponent(
                        componentConfig["type"].as<std::string>());
                component->fromFile(componentConfig);
                addComponent(component);
            }
        }

        //load children
        YAML::Node children = node["children"];
        if(children){
            for (auto i = 0; i < children.size(); ++i) {
                YAML::Node childConfig = children[i];
                std::string defaultName = "Child" + std::to_string(i);
                std::shared_ptr<GameObject> child = std::make_shared<GameObject>(defaultName);
                child->fromYamlNode(childConfig);
                children_.push_back(child);
            }
        }
    }

    void GameObject::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        //Used as a proxy for events raised from parent and affect directly to the children objects
        notify(event);
    }

    std::shared_ptr<GameObject> GameObject::findChildByName(const std::string &name) {
        for(std::shared_ptr<GameObject> child : children_){
            if(child->name_ == name)
                return child;
        }
        return std::shared_ptr<GameObject>();
    }

    void GameObject::computeTransform() {
        //remember the order of matrix multiplication is from right to left
        transform_ = glm::translate(glm::mat4(1),glm::vec3(position_.x,position_.y,0.f)) * glm::mat4_cast(glm::quat(glm::vec3(rotation_.x, rotation_.y,0.f))) * glm::scale(glm::mat4(1),glm::vec3(scale_.x, scale_.y, 1.f));
    }
}
}
