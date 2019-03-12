//
// Created by adria on 17/11/2018.
//

#include <game-engine/KeyCodes.hpp>
#include <game-engine/InputManager.hpp>
#include "StartMenuComponent.hpp"

void StartMenuComponent::init() {
    timeAcumulator_ = 0;
    text_ = gameObject()->getComponent<GameEngine::TextComponent>();
    audioSource_ = gameObject()->getComponent<GameEngine::AudioComponent>();
}

GameEngine::PropertySetBase *StartMenuComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<StartMenuComponent>(this);

    properties->add(new GameEngine::Property<StartMenuComponent, float>(
            "blinkTime",
            this,
            &StartMenuComponent::blinkTime,
            &StartMenuComponent::blinkTime,
            0));

    return properties;
}

void StartMenuComponent::Update(float elapsedTime) {

    timeAcumulator_ +=elapsedTime;
    if(timeAcumulator_>= blinkTime_) {
        if(auto text = text_.lock())
            text->setVisible(!text->isVisible());
        timeAcumulator_ = 0;
    }

    if(GameEngine::InputManager::GetInstance().isKeyDown(GameEngine::KeyCode::KEY_RETURN))
        gameObject()->game().changeScene("Scene0");


    if(GameEngine::InputManager::GetInstance().isKeyDown(GameEngine::KeyCode::KEY_SPACE)) {
        if(auto audioSource = audioSource_.lock()) {
            if(!audioSource->isPlaying())
                audioSource->play();
            else
                audioSource->stop();
        }
    }
}

void StartMenuComponent::blinkTime(const float &time) {
    blinkTime_ = time;
}

float StartMenuComponent::blinkTime() const {
    return blinkTime_;
}
