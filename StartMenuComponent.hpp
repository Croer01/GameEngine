//
// Created by adria on 17/11/2018.
//

#ifndef SPACEINVADERS_STARTMENUCOMPONENT_HPP
#define SPACEINVADERS_STARTMENUCOMPONENT_HPP


#include "src/Component.hpp"
#include "glm/glm.hpp"
#include "src/components/TextComponent.hpp"
#include "src/components/AudioComponent.hpp"

COMPONENT(StartMenuComponent)
class StartMenuComponent : public Component {
    std::weak_ptr<TextComponent> text_;
    std::weak_ptr<AudioComponent> audioSource_;
    float timeAcumulator_;
    float blinkTime_;
public:
    void init() override;

    void fromFile(const YAML::Node &componentConfig) override;

    std::shared_ptr<Component> Clone() override;

    void Update(float elapsedTime) override;
};

#endif //SPACEINVADERS_STARTMENUCOMPONENT_HPP
