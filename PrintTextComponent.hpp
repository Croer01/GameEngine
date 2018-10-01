//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_PRINTTEXTCOMPONENT_HPP
#define SPACEINVADERS_PRINTTEXTCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/ObjectManager.hpp"

COMPONENT(PrintTextComponent)
class PrintTextComponent : public Component {
    std::string text_;
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    virtual void fromFile(const INIReader &iniFile);
};


#endif //SPACEINVADERS_PRINTTEXTCOMPONENT_HPP
