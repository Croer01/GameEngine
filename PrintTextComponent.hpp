//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_PRINTTEXTCOMPONENT_HPP
#define SPACEINVADERS_PRINTTEXTCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/ObjectManager.hpp"

COMPONENT(PrintTextComponent)
class PrintTextComponent : public Component {
public:
    void Update(float elapsedTime) override;

    Component *Clone() override;
};


#endif //SPACEINVADERS_PRINTTEXTCOMPONENT_HPP
