//
// Created by adria on 08/10/2018.
//

#include "SpriteComponent.hpp"
#include "../graphics/GraphicsEngine.hpp"

void SpriteComponent::Update(float elapsedTime) {

}

std::shared_ptr<Component> SpriteComponent::Clone() {
    std::shared_ptr<SpriteComponent> clone = std::make_shared<SpriteComponent>();
    clone->graphicLoaded_ = graphicLoaded_;
    clone->graphic_ = std::make_shared<GraphicHolder>(graphicLoaded_);
    GraphicsEngine::GetInstance().registerGraphic(clone->graphic_);
    return clone;
}

void SpriteComponent::fromFile(const YAML::Node &componentConfig) {
    if(!componentConfig["filePath"])
        throw std::logic_error("property filePath not defined");
    graphicLoaded_ = std::make_shared<Graphic>(componentConfig["filePath"].as<std::string>());
}

void SpriteComponent::SetParent(GameObject *parent) {
    Component::SetParent(parent);
    if(graphic_){
        graphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());

        parent_->registerObserver(GameObjectEvent::TransformChanged,[&]{
            graphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());
        });

        parent_->registerObserver(GameObjectEvent::ActiveChanged,[&]{
            graphic_->setActive(parent_->isActive());
        });
    }
}

int SpriteComponent::getWidth() const {
    return graphicLoaded_->getWidth();
}

int SpriteComponent::getHeight() const {
    return graphicLoaded_->getHeight();
}
