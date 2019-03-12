//
// Created by adria on 25/11/2018.
//

#ifndef SPACEINVADERS_BLINKTEXTCOMPONENT_HPP
#define SPACEINVADERS_BLINKTEXTCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/components/TextComponent.hpp>

COMPONENT(BlinkTextComponent)
class BlinkTextComponent : public GameEngine::geComponentInstantiable<BlinkTextComponent> {
    float duration_;
    float blinkTimes_;
    float timeToBlinkAcumulator_;
    float durationAcumulator_;
    float blinkElapsedTime_;
    std::weak_ptr<GameEngine::TextComponent> text_;
public:
    void init() override;

    void Update(float elapsedTime) override;

    void blinkTimes(const float &times);
    float blinkTimes() const;

    void duration(const float &dur);
    float duration() const;

protected:
    GameEngine::PropertySetBase *configureProperties() override;
};


#endif //SPACEINVADERS_BLINKTEXTCOMPONENT_HPP
