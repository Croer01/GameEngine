//
// Created by adria on 08/10/2018.
//

#include "SpriteComponent.hpp"

void SpriteComponent::Update(float elapsedTime) {

}

std::shared_ptr<Component> SpriteComponent::Clone() {
    return std::shared_ptr<Component>();
}

void SpriteComponent::fromFile(const INIReader &iniFile) {
    Component::fromFile(iniFile);
}
