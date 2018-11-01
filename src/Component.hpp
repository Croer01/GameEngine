//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_COMPONENT_HPP
#define SPACEINVADERS_COMPONENT_HPP


#include <yaml-cpp/yaml.h>
#include "GameObject.hpp"

#define COMPONENT(x) //the #x component has been registered

class Component {
protected:
    GameObject *parent_;
public:
    virtual ~Component() = default;
    virtual void Update(float elapsedTime){};
    virtual void init(){};
    virtual std::shared_ptr<Component> Clone() = 0;
    virtual void fromFile(const YAML::Node &componentConfig){};

    virtual void SetParent(GameObject *parent);
    GameObject *getParent();

};


#endif //SPACEINVADERS_COMPONENT_HPP
