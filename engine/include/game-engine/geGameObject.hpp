//
// Created by adria on 23/01/2019.
//

#ifndef SPACEINVADERS_GEGAMEOBJECT_HPP
#define SPACEINVADERS_GEGAMEOBJECT_HPP

#include <game-engine/api.hpp>
#include <memory>
#include <vector>
#include <string>
#include <game-engine/events/Subject.hpp>

namespace GameEngine {

    class geComponent;
    class UIControlComponent;
    class Game;

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

        virtual float rotation() const = 0;
        virtual void rotation(float rotation) = 0;

        virtual Vec2D scale() const = 0;
        virtual void scale(const Vec2D &scale) = 0;

        virtual bool active() const = 0;
        virtual void active(bool isActive) = 0;

        virtual void parent(const geGameObjectRef &gameObject) = 0;
        virtual std::weak_ptr<geGameObject> parent() const = 0;

        virtual void addComponent(const std::shared_ptr<geComponent> &component) = 0;

        virtual geGameObjectRef findChildByName(const std::string &name) = 0;

        virtual Game *game() const = 0;

        virtual bool isDestroyed() const = 0;
        virtual void destroy() = 0;

        template <typename T>
        std::weak_ptr<T> getUIComponent(const std::string &controlId) const {
            if(!std::is_base_of<UIControlComponent,T>::value)
                throw std::invalid_argument("the type of the component doesn't inherit from UIControlComponent");

            for(auto component : components_){
                if(auto desiredUIComponent = std::dynamic_pointer_cast<UIControlComponent>(component))
                {
                    if(controlId == desiredUIComponent->id())
                    {
                        if (auto desiredComponent = std::dynamic_pointer_cast<T>(component))
                            return desiredComponent;
                    }
                }
            }

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
    private:
    virtual void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) = 0;
    };

}
#endif //SPACEINVADERS_GEGAMEOBJECT_HPP
