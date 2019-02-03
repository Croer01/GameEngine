//
// Created by adria on 23/01/2019.
//

#ifndef SPACEINVADERS_GEGAMEOBJECT_HPP
#define SPACEINVADERS_GEGAMEOBJECT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <memory>
#include <vector>
#include <string>

namespace GameEngine {

    enum class GameObjectEvent{
        TransformChanged,
        PositionChanged,
        RotationChanged,
        ScaleChanged,
        ActiveChanged
    };
    class geGameObject;

    typedef std::shared_ptr<geGameObject> geGameObjectRef;

    class PUBLICAPI geGameObject {
    public:
        virtual ~geGameObject(){};

        virtual std::string name() const = 0;
        virtual void name(const std::string &name) = 0;

        virtual Vec2D position() const = 0;
        virtual void position(const Vec2D &position) = 0;

        virtual Vec2D rotation() const = 0;
        virtual void rotation(const Vec2D &rotation) = 0;

        virtual Vec2D scale() const = 0;
        virtual void scale(const Vec2D &scale) = 0;

        virtual bool active() const = 0;
        virtual void active(bool isActive) = 0;

        virtual void parent(const geGameObjectRef &gameObject) = 0;

        virtual void addComponent(const geComponentRef &component) = 0;

        template <typename T>
        std::weak_ptr<T> getComponent() const {
            if(!std::is_base_of<geComponent,T>::value)
                throw std::invalid_argument("the type of the component doesn't inherit from geComponent");

            for(auto component : components_){
                if(auto desiredComponent = std::dynamic_pointer_cast<T>(component))
                    return desiredComponent;
            }

            return std::shared_ptr<T>();
        }
    protected:
        std::vector<geComponentRef> components_;

    };

}
#endif //SPACEINVADERS_GEGAMEOBJECT_HPP
