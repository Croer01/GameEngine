//
// Created by adria on 08/10/2018.
//

#include "SpriteComponent.hpp"
#include "../graphics/GraphicsEngine.hpp"

void SpriteComponent::Update(float elapsedTime) {

}

std::shared_ptr<Component> SpriteComponent::Clone() {
    std::shared_ptr<SpriteComponent> clone = std::make_shared<SpriteComponent>();
    clone->graphic_ = graphic_;
    GraphicsEngine::GetInstance().registerGraphic(graphic_);
    return clone;
}

void SpriteComponent::fromFile(const YAML::Node &componentConfig) {
    if(!componentConfig["filePath"])
        throw std::logic_error("property filePath not defined");
    graphic_ = std::make_shared<Graphic>(componentConfig["filePath"].as<std::string>());
}

void SpriteComponent::SetParent(GameObject *parent) {
    Component::SetParent(parent);
    graphic_->setModelTransform(parent_->getTransform());
    parent_->registerObserver(GameObjectEvent::TransformChanged,[&]{
        graphic_->setModelTransform(parent_->getTransform());
    });
}
