//
// Created by adria on 04/11/2018.
//

#include "TextComponent.hpp"
#include "../graphics/font/FontManager.hpp"
#include "../graphics/GraphicsEngine.hpp"

std::shared_ptr<Component> TextComponent::Clone() {
    std::shared_ptr<TextComponent> clone = std::make_shared<TextComponent>();
    clone->text_ = text_;
    clone->font_ = font_;
    clone->fontSize_ = fontSize_;

    clone->textGraphic_ = FontManager::GetInstance().getFont(clone->font_, clone->fontSize_)->createText(clone->text_);
    GraphicsEngine::GetInstance().registerText(clone->textGraphic_);

    return clone;
}

void TextComponent::fromFile(const YAML::Node &componentConfig) {
    text_ = componentConfig["text"].as<std::string>();
    font_ = componentConfig["font"].as<std::string>();
    fontSize_ = componentConfig["fontSize"].as<unsigned int>();
}

void TextComponent::SetParent(GameObject *parent) {
    if(parent_)
        parent_->unregisterObserver(this);
    parent_ = parent;

    if(parent_ && textGraphic_){
        textGraphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());

        parent_->registerObserver(this);
    }
}

void TextComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::TransformChanged){
        textGraphic_->setModelTransform(parent_->getPosition(), parent_->getRotation(), parent_->getScale());
    }
    else if(event == GameObjectEvent::ActiveChanged){
        textGraphic_->setActive(parent_->isActive());
    }
}

void TextComponent::setText(const std::string &text) {
    text_ = text;
    textGraphic_->setText(text_);
}
