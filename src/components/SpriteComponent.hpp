//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include "../Component.hpp"

class SpriteComponent : public Component {
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const INIReader &iniFile) override;
};


#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
