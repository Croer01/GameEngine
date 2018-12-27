//
// Created by adria on 25/11/2018.
//

#ifndef SPACEINVADERS_BLINKTEXTCOMPONENT_HPP
#define SPACEINVADERS_BLINKTEXTCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/components/TextComponent.hpp"

COMPONENT(BlinkTextComponent)
class BlinkTextComponent : public Component {
    float duration_;
    float blinkTimes_;
    float timeToBlinkAcumulator_;
    float durationAcumulator_;
    float blinkElapsedTime_;
    std::weak_ptr<TextComponent> text_;
public:
    void init() override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void Update(float elapsedTime) override;
};


#endif //SPACEINVADERS_BLINKTEXTCOMPONENT_HPP
