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
    return clone;
}

void SpriteComponent::fromFile(const INIReader &iniFile) {
    std::string filePath = iniFile.Get("SpriteComponent","filePath","[Text not found]");

    graphic_ = std::make_shared<Graphic>(filePath);
    //TODO: change this if the graphic is cloned, currently this doesn't happened
    GraphicsEngine::GetInstance().registerGraphic(graphic_);
}

void SpriteComponent::SetParent(GameObject *parent) {
    Component::SetParent(parent);
    graphic_->setModelTransform(parent_->getTransform());
    parent_->registerObserver(GameObjectEvent::TransformChanged,[&]{
        graphic_->setModelTransform(parent_->getTransform());
    });
}
