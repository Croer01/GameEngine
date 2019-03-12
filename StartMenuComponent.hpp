//
// Created by adria on 17/11/2018.
//

#ifndef SPACEINVADERS_STARTMENUCOMPONENT_HPP
#define SPACEINVADERS_STARTMENUCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/AudioComponent.hpp>

COMPONENT(StartMenuComponent)
class StartMenuComponent : public GameEngine::geComponentInstantiable<StartMenuComponent> {
    std::weak_ptr<GameEngine::TextComponent> text_;
    std::weak_ptr<GameEngine::AudioComponent> audioSource_;
    float timeAcumulator_;
    float blinkTime_;
public:
    void init() override;

    void Update(float elapsedTime) override;

    virtual GameEngine::PropertySetBase *configureProperties();

    void blinkTime(const float &time);
    float blinkTime() const;
};

#endif //SPACEINVADERS_STARTMENUCOMPONENT_HPP
