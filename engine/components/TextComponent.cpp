//
// Created by adria on 04/11/2018.
//

#include <game-engine/components/TextComponent.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
    void TextComponent::preInit()
    {
        graphicsEngine_ = gameObject()->game()->graphicsEngine();
    }

    void TextComponent::init() {
        setPropertyObserver<std::string>("font", [this](){ updateFont(); });
        setPropertyObserver<int>("fontSize", [this](){ updateFontSize(); });
        setPropertyObserver<std::string>("text", [this](){ updateText(); });

        visible_ = true;

        updateTextRef();

        updateTextTransform();
        gameObject()->registerObserver(this);
    }

    void TextComponent::onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) {
        if (event == GameObjectEvent::TransformChanged) {
            updateTextTransform();
        } else if (event == GameObjectEvent::ActiveChanged) {
            textGraphic_->setActive(gameObject()->active() && visible_);
        }
    }
    void TextComponent::updateFont() {
        textParams_.fontName = getPropertyValue<std::string>("font");

        if(textGraphic_)
            updateTextRef();
    }

    void TextComponent::updateFontSize() {
        int size = getPropertyValue<int>("fontSize");
        if(size < 0)
            throw std::invalid_argument("font size must be greater than 0. current value is " + std::to_string(size));
        textParams_.fontSize = size;
        if(textGraphic_)
            updateTextRef();
    }

    void TextComponent::updateText() {
        textParams_.text = getPropertyValue<std::string>("text");

        if(textGraphic_ )
            textGraphic_->setText(textParams_.text);
    }

    TextComponent::~TextComponent() {
        if(graphicsEngine_ != nullptr && textGraphic_)
            graphicsEngine_->unregisterText(textGraphic_);
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
            oldGameObject->unregisterObserver(this);

        if (newGameObject && textGraphic_) {
            updateTextTransform();
            newGameObject->registerObserver(this);
        }
    }

    void TextComponent::updateTextRef() {
        Game *game = gameObject()->game();
        if(game == nullptr)
            return;

        if(textGraphic_)
            graphicsEngine_->unregisterText(textGraphic_);

        if(!textParams_.fontName.empty()){
            textGraphic_ = game->fontManager()->getFont(textParams_.fontName, textParams_.fontSize)->createText(textParams_.text);
        }else{
            textGraphic_ = game->fontManager()->getDefaultFont(textParams_.fontSize)->createText(textParams_.text);
        }
        graphicsEngine_->registerText(textGraphic_);
    }
}
