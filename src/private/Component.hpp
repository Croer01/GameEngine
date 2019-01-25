//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_COMPONENT_HPP
#define SPACEINVADERS_COMPONENT_HPP


#include <yaml-cpp/yaml.h>
#include "GameObject.hpp"

namespace GameEngine {
namespace Internal {
    #define COMPONENT(x) //the #x component has been registered

    class Component {
        GameObject *gameObject_;
    protected:
        virtual void onGameObjectChange(GameObject *oldGameObject, GameObject *newGameObject) {};
    public:
        virtual ~Component(){};
        virtual void Update(float elapsedTime){};
        virtual void init(){};
        virtual std::shared_ptr<Component> Clone() = 0;
        virtual void fromFile(const YAML::Node &componentConfig){};

        void gameObject(GameObject *gameObject);
        GameObject *gameObject() const;

    };
}
}


#endif //SPACEINVADERS_COMPONENT_HPP
