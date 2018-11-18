//
// Created by adria on 17/11/2018.
//

#include "StartMenuComponent.hpp"
#include "src/InputManager.hpp"
#include "src/SceneManager.hpp"

void StartMenuComponent::init() {
    timeAcumulator_ = 0;
    text_ = parent_->getComponent<TextComponent>();
}

void StartMenuComponent::Update(float elapsedTime) {

    timeAcumulator_ +=elapsedTime;
    if(timeAcumulator_>= blinkTime_) {
        if(auto text = text_.lock())
            text->setVisible(!text->isVisible());
        timeAcumulator_ = 0;
    }

    if(InputManager::GetInstance().isKeyDown(SDLK_SPACE))
        SceneManager::GetInstance().changeScene("Scene0");
}

std::shared_ptr<Component> StartMenuComponent::Clone() {
    std::shared_ptr<StartMenuComponent> clone = std::make_shared<StartMenuComponent>();
    clone->blinkTime_ = blinkTime_;
    return clone;
}

void StartMenuComponent::fromFile(const YAML::Node &componentConfig) {
    blinkTime_ = componentConfig["blinkTime"].as<float>();
}
