//
// Created by adria on 04/11/2018.
//

#include <game-engine/components/TextComponent.hpp>
#include "../private/graphics/font/FontManager.hpp"
#include "../private/graphics/GraphicsEngine.hpp"
#include "../../private/GameObject.hpp"

namespace GameEngine {
    void TextComponent::init() {
        visible_ = true;

        updateTextRef();

        updateTextTransform();
    }

    PropertySetBase *TextComponent::configureProperties() {
        auto *properties = new PropertySet<TextComponent>(this);

        properties->add(new Property<TextComponent, std::string>(
                "font",
                this,
                &TextComponent::font,
                &TextComponent::font,
                "",
                true));

        properties->add(new Property<TextComponent, int>(
                "fontSize",
                this,
                &TextComponent::fontSize,
                &TextComponent::fontSize,
                0));

        properties->add(new Property<TextComponent, std::string>(
                "text",
                this,
                &TextComponent::text,
                &TextComponent::text,
                ""));

        return properties;
    }

    void TextComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if (event == GameObjectEvent::TransformChanged) {
            updateTextTransform();
        } else if (event == GameObjectEvent::ActiveChanged) {
            textGraphic_->setActive(gameObject()->active() && visible_);
        }
    }
    void TextComponent::font(const std::string &fontName) {
        textParams_.fontName = fontName;

        if(textGraphic_)
            updateTextRef();
    }

    std::string TextComponent::font() const {
        return textParams_.fontName;
    }

    void TextComponent::fontSize(const int &size) {
        if(size < 0)
            throw std::invalid_argument("fontsize must be greater than 0. current value is " + std::to_string(size));
        textParams_.fontSize = size;
        if(textGraphic_)
            updateTextRef();
    }

    int TextComponent::fontSize() const {
        return textParams_.fontSize;
    }
    void TextComponent::text(const std::string &text) {
        textParams_.text = text;

        if(textGraphic_ )
            textGraphic_->setText(textParams_.text);
    }

    std::string TextComponent::text() const {
        return textParams_.text;
    }

    TextComponent::~TextComponent() {
        if(textGraphic_)
            Internal::GraphicsEngine::GetInstance().unregisterText(textGraphic_);
    }

    void TextComponent::setVisible(bool visible) {
        visible_ = visible;
        textGraphic_->setActive(gameObject()->active() && visible_);
    }

    bool TextComponent::isVisible() const {
        return visible_;
    }

    void TextComponent::updateTextTransform() {
        textGraphic_->setModelTransform(gameObject()->position(),
                gameObject()->rotation(),
                gameObject()->scale());
    }

    void TextComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            dynamic_cast<Internal::GameObject*>(oldGameObject)->unregisterObserver(this);

        if (newGameObject && textGraphic_) {
            updateTextTransform();
            dynamic_cast<Internal::GameObject*>(newGameObject)->registerObserver(this);
        }
    }

    void TextComponent::updateTextRef() {
        if(textGraphic_)
            Internal::GraphicsEngine::GetInstance().unregisterText(textGraphic_);

        if(!textParams_.fontName.empty()){
            textGraphic_ = Internal::FontManager::GetInstance().getFont(textParams_.fontName, textParams_.fontSize)->createText(textParams_.text);
            Internal::GraphicsEngine::GetInstance().registerText(textGraphic_);
        }else{
            textGraphic_.reset();
        }
    }
}
