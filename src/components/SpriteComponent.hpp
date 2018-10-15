//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include "../Component.hpp"
#include "../graphics/Graphic.hpp"

class SpriteComponent : public Component {
    std::shared_ptr<Graphic> graphic_;
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const INIReader &iniFile) override;

    void SetParent(GameObject *parent) override;
};


#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
