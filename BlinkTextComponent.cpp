//
// Created by adria on 25/11/2018.
//

#include "BlinkTextComponent.hpp"

void BlinkTextComponent::init() {
    timeToBlinkAcumulator_ = 0;
    durationAcumulator_ = 0;
    blinkElapsedTime_ = duration_/blinkTimes_;
    text_ = gameObject()->getComponent<TextComponent>();
}

std::shared_ptr<Component> BlinkTextComponent::Clone() {
    auto clone = std::make_shared<BlinkTextComponent>();
    clone->duration_ = duration_;
    clone->blinkTimes_ = blinkTimes_;
    return clone;
}

void BlinkTextComponent::fromFile(const YAML::Node &componentConfig) {
    duration_ = componentConfig["duration"].as<float>(0);
    blinkTimes_ = componentConfig["blinkTimes"].as<float>(0);
}

void BlinkTextComponent::Update(float elapsedTime) {
    timeToBlinkAcumulator_ += elapsedTime;
    durationAcumulator_ += elapsedTime;

    if(timeToBlinkAcumulator_ >= blinkElapsedTime_) {
        if(auto text = text_.lock())
            text->setVisible(!text->isVisible());
        timeToBlinkAcumulator_ = 0;
    }

    if(durationAcumulator_ >= duration_) {
        gameObject()->setActive(false);
        durationAcumulator_ = 0;
    }
}
