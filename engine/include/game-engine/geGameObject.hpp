//
// Created by adria on 23/01/2019.
//

#ifndef SPACEINVADERS_GEGAMEOBJECT_HPP
#define SPACEINVADERS_GEGAMEOBJECT_HPP

#include <game-engine/api.hpp>
#include <game-engine/events/Subject.hpp>
#include <game-engine/geComponent.hpp>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

namespace GameEngine {

    class Game;
    class UIControlComponent;

enum class GameObjectEvent{
        TransformChanged,
        PositionChanged,
        RotationChanged,
        ScaleChanged,
        ActiveChanged
    };

    class geGameObject;
    typedef std::shared_ptr<geGameObject> geGameObjectRef;

    class PUBLICAPI geGameObject : public Subject<GameObjectEvent> {
    public:
        virtual ~geGameObject(){};

        virtual std::string name() const = 0;
        virtual void name(const std::string &name) = 0;

        virtual Vec2D position() const = 0;
        virtual Vec2D localPosition() const = 0;
        virtual void position(const Vec2D &position) = 0;
        virtual Vec2D transformToLocalPosition(const Vec2D &position) const = 0;
        virtual Vec2D transformToWorldPosition(const Vec2D &position) const = 0;

        virtual float rotation() const = 0;
        virtual void rotation(float rotation) = 0;

        virtual Vec2D scale() const = 0;
        virtual void scale(const Vec2D &scale) = 0;

        virtual bool active() const = 0;
        virtual void active(bool isActive) = 0;

        virtual void parent(geGameObject *gameObject) = 0;
        virtual geGameObject *parent() const = 0;

        virtual void addComponent(const std::shared_ptr<geComponent> &component) = 0;

        virtual geGameObjectRef findChildByName(const std::string &name) = 0;

        virtual Game *game() const = 0;

        std::vector<std::weak_ptr<geComponent>> getAllComponents() const
        {
            std::vector<std::weak_ptr<geComponent>> allComponents;
            allComponents.reserve(components_.size());

            for(const auto& comp : components_)
            {
                allComponents.emplace_back(comp);
            }

            return allComponents;
        }

        template <typename T>
        std::weak_ptr<T> getUIComponent(const std::string &controlId) const {
            for(auto component : components_){
                if(std::dynamic_pointer_cast<UIControlComponent>(component))
                {
                    if(controlId == component->getPropertyValue<std::string>("id"))
                    {
                        if (auto desiredComponent = std::dynamic_pointer_cast<T>(component))
                            return desiredComponent;
                    }
                }
            }

            throw std::invalid_argument("the type of the component doesn't inherit from UIControlComponent");

            return std::shared_ptr<T>();
        }

        template <typename T>
        std::weak_ptr<T> getComponent() const {
            if(!std::is_base_of<geComponent,T>::value)
                throw std::invalid_argument("the type of the component doesn't inherit from geComponent");

            if(std::is_base_of<UIControlComponent,T>::value)
                throw std::invalid_argument("Use \"getUIComponent\" to get a UI component");

            for(auto component : components_){
                if(auto desiredComponent = std::dynamic_pointer_cast<T>(component))
                    return desiredComponent;
            }

            return std::shared_ptr<T>();
        }
    protected:
        std::vector<std::shared_ptr<geComponent>> components_;

    };
}
#endif //SPACEINVADERS_GEGAMEOBJECT_HPP
