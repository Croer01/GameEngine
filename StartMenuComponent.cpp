//
// Created by adria on 17/11/2018.
//

#include "StartMenuComponent.hpp"
#include "src/InputManager.hpp"
#include "src/SceneManager.hpp"

std::shared_ptr<Component> StartMenuComponent::Clone() {
    std::shared_ptr<StartMenuComponent> clone = std::make_shared<StartMenuComponent>();
    return clone;
}

void StartMenuComponent::Update(float elapsedTime) {
    if(InputManager::GetInstance().isKeyDown(SDLK_SPACE))
        SceneManager::GetInstance().changeScene("Scene0");
}

