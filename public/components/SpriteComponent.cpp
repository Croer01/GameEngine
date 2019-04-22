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

    PropertySetBase *SpriteComponent::configureProperties() {
        auto *properties = new PropertySet<SpriteComponent>(this);

        properties->add(new Property<SpriteComponent, std::string>(
                        "filePath",
                        this,
                        &SpriteComponent::filepath,
                        &SpriteComponent::filepath,
                        "",
                        true));
        properties->add(new Property<SpriteComponent, bool>(
                        "visible",
                        this,
                        &SpriteComponent::isVisible,
                        &SpriteComponent::setVisible,
                        true));

        properties->add(new Property<SpriteComponent, std::string>(
                "anchor",
                this,
                &SpriteComponent::anchor,
                &SpriteComponent::anchor,
                "",
                false));
        return properties;
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
        if(filePath_.empty()){
            if(graphic_)
                Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);

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

        GameEngine::Internal::GraphicAnchor graphicAnchor;

        if(anchor == "TOP_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_LEFT;
        else if(anchor == "TOP_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_CENTER;
        else if(anchor == "TOP_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_RIGHT;
        else if(anchor == "MIDDLE_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_LEFT;
        else if(anchor == "MIDDLE_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_CENTER;
        else if(anchor == "MIDDLE_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::MIDDLE_RIGHT;
        else if(anchor == "BOTTOM_LEFT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_LEFT;
        else if(anchor == "BOTTOM_CENTER")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_CENTER;
        else if(anchor == "BOTTOM_RIGHT")
            graphicAnchor = GameEngine::Internal::GraphicAnchor::BOTTOM_RIGHT;
        else
            graphicAnchor = GameEngine::Internal::GraphicAnchor::TOP_LEFT;

        if(graphic_)
            graphic_->setAnchor(graphicAnchor);

        anchor_ = anchor;
    }
}