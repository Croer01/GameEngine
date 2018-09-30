//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_COMPONENT_HPP
#define SPACEINVADERS_COMPONENT_HPP


#include <INIReader.h>
#include "GameObject.hpp"

#define COMPONENT(x) //the #x component has been registered

class Component {
protected:
    GameObject *parent_;
public:
    virtual ~Component() = default;
    virtual void Update(float elapsedTime) = 0;
    virtual std::shared_ptr<Component> Clone() = 0;
    virtual void fromFile(INIReader *iniFile);
    void SetParent(GameObject *parent);

};


#endif //SPACEINVADERS_COMPONENT_HPP
