//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include "Component.hpp"

class ComponentBuilder {
public:
    virtual ~ComponentBuilder() = default;
    virtual Component* Create() = 0;
};

template <typename ComponentType>
class ComponentTBuilder : public ComponentBuilder{
public:
    virtual Component* Create(){
        return new ComponentType();
    };
};


#endif //SPACEINVADERS_BUILDER_HPP
