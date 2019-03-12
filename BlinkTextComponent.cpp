//
// Created by adria on 25/11/2018.
//

#include "BlinkTextComponent.hpp"

void BlinkTextComponent::init() {
    timeToBlinkAcumulator_ = 0;
    durationAcumulator_ = 0;
    blinkElapsedTime_ = duration_/blinkTimes_;
    text_ = gameObject()->getComponent<GameEngine::TextComponent>();
}

GameEngine::PropertySetBase *BlinkTextComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<BlinkTextComponent>(this);

    properties->add(new GameEngine::Property<BlinkTextComponent, float>(
            "duration",
            this,
            &BlinkTextComponent::duration,
            &BlinkTextComponent::duration,
            0));

    properties->add(new GameEngine::Property<BlinkTextComponent, float>(
            "blinkTimes",
            this,
            &BlinkTextComponent::blinkTimes,
            &BlinkTextComponent::blinkTimes,
            0));

    return properties;
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
        gameObject()->active(false);
        durationAcumulator_ = 0;
    }
}

void BlinkTextComponent::blinkTimes(const float &times) {
    blinkTimes_ = times;
}

float BlinkTextComponent::blinkTimes() const {
    return blinkTimes_;
}


void BlinkTextComponent::duration(const float &dur) {
    duration_ = dur;
}

float BlinkTextComponent::duration() const {
    return duration_;
}