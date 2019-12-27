//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include <sstream>
#include "GameObject.hpp"
#include "Game.hpp"
#include "yamlConverters.hpp"
#include <game-engine/geComponent.hpp>
#include "ObjectManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace GameEngine {
namespace Internal {
    int GameObject::ID_GENERATOR = 0;

    GameObject::GameObject(const std::string &prototype) : Observer<GameObjectEvent>(),
            prototype_(prototype),
            id_(ID_GENERATOR++),
            active_(true),
            activeValueToSetInSafeMode_(true),
            hasActiveValueToSetInSafeMode_(false),
            position_(0.f, 0.f),
            rotation_(0.f, 0.f),
            scale_(1.f,1.f),
            destroyed_(false),
            initializating_(false){
        computeTransform();
    }

    void GameObject::Init() {
        initializating_ = true;
        std::cout << "Object " << prototype_.c_str() << " initialized" << std::endl;
        // Use conventional for-loop to avoid access violation of components have added by others components in init time
        auto componentSize = components_.size();
        for (auto i = 0; i < componentSize; i++)
        {
            components_[i]->init();
        }

        for (auto &child : children_)
        {
            child->Init();
        }

        initializating_ = false;
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

    void GameObject::addComponent(const geComponentRef &component) {
        auto it = std::find(components_.begin(),components_.end(),component);
        if(it != components_.end())
            return;
        component->gameObject(this);
        components_.push_back(component);
        //Ensure the component is initializated if this is added during the GameObject's init process
        if(initializating_)
            component->init();
    }

    void GameObject::addChild(std::shared_ptr<GameObject> child) {
        auto it = std::find(children_.begin(),children_.end(),child);
        if(it != children_.end())
            return;
        child->parent(shared_from_this());
        children_.push_back(child);
    }

    void GameObject::parent(const geGameObjectRef &goParent) {
        if(auto parent = parent_.lock())
            parent->unregisterObserver(this);
        parent_.reset();
        //goParent could be null
        if(goParent) {
            parent_ = std::dynamic_pointer_cast<GameObject>(goParent);
            parent_.lock()->registerObserver(this);
        }
        notify(GameObjectEvent::TransformChanged);
    }

    GameObject::~GameObject(){
        if(!destroyed_)
            std::cerr << "You are trying to destroy a GameObject that still alive in current Scene" << std::endl;
        for (auto &component : components_) {
            component->gameObject(nullptr);
        }
        components_.clear();

        for (auto &child : children_) {
            child->parent(nullptr);
        }
        children_.clear();
    }

    std::shared_ptr<GameObject> GameObject::Clone() const {
        auto cloned = std::make_shared<GameObject>(prototype_);

        cloned->name_ = name_;
        cloned->position_ = position_;
        cloned->rotation_ = rotation_;
        cloned->scale_ = scale_;

        for (auto &component : components_) {
            std::shared_ptr<geComponent> clonedComponent = component->clone();
            if(!clonedComponent)
                throw std::runtime_error("one of the components of " + prototype_ + " has an error during cloning process");
            cloned->addComponent(clonedComponent);
        }

        for (auto &child : children_) {
            std::shared_ptr<GameObject> clonedChild = child->Clone();
            if(!clonedChild)
                throw std::runtime_error("one of the child of " + prototype_ + " has an error during cloning process");
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
        if(auto parent = parent_.lock())
            position += parent->position_;
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
        if(auto parent = parent_.lock())
            rotation += parent->rotation_;
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
        if(auto parent = parent_.lock())
            scale *= parent->scale_;
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
        if(auto parent = parent_.lock())
            return parent->getTransform() * transform_;

        return transform_;
    }

    bool GameObject::active() const {
        return activeValueToSetInSafeMode_;
    }

    void GameObject::active(bool active) {
        activeValueToSetInSafeMode_ = active;
        hasActiveValueToSetInSafeMode_ = true;
    }

    std::string GameObject::name() const {
        return name_;
    }

    void GameObject::name(const std::string &name) {
        name_ = name;
    }

    void GameObject::fromYamlNode(const YAML::Node &node) {
        if(node["name"])
            name_ = node["name"].as<std::string>();

        //object properties
        if(node["position"])
            this->position(node["position"].as<Vec2D>());

        if(node["rotation"])
            this->rotation(node["rotation"].as<Vec2D>());

        if(node["scale"])
            this->scale(node["scale"].as<Vec2D>());

        //load components
        YAML::Node components = node["components"];
        if(components) {
            for (auto i = 0; i < components.size(); ++i) {
                YAML::Node componentConfig = components[i];
                geComponentRef component = ObjectManager::GetInstance().createComponent(
                        componentConfig["type"].as<std::string>(),componentConfig);

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

    GameEngine::geGameObjectRef GameObject::findChildByName(const std::string &name) {
        for(std::shared_ptr<GameObject> child : children_){
            if(child->name_ == name)
                return child;
        }
        return geGameObjectRef();
    }

    void GameObject::computeTransform() {
        //remember the order of matrix multiplication is from right to left
        transform_ = glm::translate(glm::mat4(1),glm::vec3(position_.x,position_.y,0.f)) * glm::mat4_cast(glm::quat(glm::vec3(rotation_.x, rotation_.y,0.f))) * glm::scale(glm::mat4(1),glm::vec3(scale_.x, scale_.y, 1.f));
    }

    geGame &GameObject::game() const {
        return Internal::Game::GetInstance().context();
    }

    bool GameObject::isDestroyed() const {
        return destroyed_;
    }

    void GameObject::destroy() {
        destroyed_ = true;
    }
}
}
