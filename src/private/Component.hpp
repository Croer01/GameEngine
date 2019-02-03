//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_COMPONENT_HPP
#define SPACEINVADERS_COMPONENT_HPP


#include <yaml-cpp/yaml.h>
#include <game-engine/geComponent.hpp>
#include "GameObject.hpp"

namespace GameEngine {
namespace Internal {
    #define COMPONENT(x) //the #x component has been registered

    class Component : public geComponent {
        GameEngine::geGameObject *gameObject_;
    protected:
        virtual void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {};
    public:
        virtual ~Component(){};
        virtual void Update(float elapsedTime){};
        virtual void init(){};
        virtual std::shared_ptr<Component> Clone() = 0;
        virtual void fromFile(const YAML::Node &componentConfig){};

        virtual void gameObject(GameEngine::geGameObject *gameObject);
        virtual GameEngine::geGameObject *gameObject() const;

    };
}
}


#endif //SPACEINVADERS_COMPONENT_HPP
