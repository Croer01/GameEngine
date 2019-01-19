//
// Created by adria on 04/11/2018.
//

#include "TextComponent.hpp"
#include "../graphics/font/FontManager.hpp"
#include "../graphics/GraphicsEngine.hpp"

void TextComponent::init() {
    visible_ = true;
    updateTextTransform();
}

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
    text_ = componentConfig["text"].as<std::string>("");
    font_ = componentConfig["font"].as<std::string>();
    fontSize_ = componentConfig["fontSize"].as<unsigned int>();
}

void TextComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
    if(event == GameObjectEvent::TransformChanged){
        updateTextTransform();
    }
    else if(event == GameObjectEvent::ActiveChanged){
        textGraphic_->setActive(gameObject()->isActive() && visible_);
    }
}

void TextComponent::setText(const std::string &text) {
    text_ = text;
    textGraphic_->setText(text_);
}

TextComponent::~TextComponent() {
    GraphicsEngine::GetInstance().unregisterText(textGraphic_);
}

void TextComponent::setVisible(bool visible) {
    visible_ = visible;
    textGraphic_->setActive(gameObject()->isActive() && visible_);
}

bool TextComponent::isVisible() const {
    return visible_;
}

void TextComponent::updateTextTransform() {
    const glm::vec3 &position = gameObject()->getPosition();
    const glm::vec3 &rotation = gameObject()->getRotation();
    const glm::vec3 &scale = gameObject()->getScale();
    textGraphic_->setModelTransform(position, rotation, scale);
}

void TextComponent::onGameObjectChange(GameObject *oldGameObject, GameObject *newGameObject) {
    if(oldGameObject)
        oldGameObject->unregisterObserver(this);

    if(newGameObject && textGraphic_){
        updateTextTransform();
        newGameObject->registerObserver(this);
    }
}
