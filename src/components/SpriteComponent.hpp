//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include "../Component.hpp"
#include "../graphics/Graphic.hpp"
#include "../graphics/GraphicHolder.hpp"

class SpriteComponent : public Component {
    std::shared_ptr<Graphic> graphicLoaded_;
    std::shared_ptr<GraphicHolder> graphic_;
public:
    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void SetParent(GameObject *parent) override;

    int getWidth() const;
    int getHeight() const;
};


#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
