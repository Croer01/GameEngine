//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteComponent.hpp>
#include "../private/graphics/GraphicsEngine.hpp"
#include "../private/GameObject.hpp"
#include "../../private/graphics/GraphicSprite.hpp"

namespace GameEngine {
    void SpriteComponent::init() {
        updateGraphicRef();
        setVisible(visible_);
        anchor(anchor_);
        graphic_->setModelTransform(gameObject()->position(),gameObject()->rotation(),gameObject()->scale());
        gameObject()->registerObserver(this);
    }

    int SpriteComponent::getWidth() const {
        return graphicLoaded_->getWidth();
    }

    int SpriteComponent::getHeight() const {
        return graphicLoaded_->getHeight();
    }

    void SpriteComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if (event == GameObjectEvent::TransformChanged) {
            graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(),
                                        gameObject()->scale());
        } else if (event == GameObjectEvent::ActiveChanged) {
            graphic_->setActive(gameObject()->active() && visible_);
        }
    }

    SpriteComponent::~SpriteComponent() {
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
    }

    void SpriteComponent::setVisible(const bool &visible) {
        visible_ = visible;
        if(graphic_)
            graphic_->setActive(gameObject() && gameObject()->active() && visible_);
    }

    bool SpriteComponent::isVisible() const {
        return visible_;
    }

    void SpriteComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(newGameObject->position(), newGameObject->rotation(),
                                        newGameObject->scale());

            newGameObject->registerObserver(this);
        }
    }

    void SpriteComponent::filepath(const std::string &path) {
        filePath_ = path;

        if(graphic_)
            updateGraphicRef();
    }

    std::string SpriteComponent::filepath() const {
        return filePath_;
    }

    void SpriteComponent::updateGraphicRef() {
        if(graphic_)
            Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);

        if(filePath_.empty()){
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath_);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            Internal::GraphicsEngine::GetInstance().registerGraphic(graphic_);
        }
    }

    std::string SpriteComponent::anchor() const {
        return anchor_;
    }

    void SpriteComponent::anchor(const std::string &anchor) {
        if(graphic_)
            graphic_->setAnchor(Internal::parseStringToGraphicAnchor(anchor));

        anchor_ = anchor;
    }
}