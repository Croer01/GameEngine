//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteComponent.hpp>
#include "../private/graphics/GraphicsEngine.hpp"
#include "../private/GameObject.hpp"

namespace GameEngine {
    void SpriteComponent::init() {
        setVisible(visible_);
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
            dynamic_cast<Internal::GameObject*>(oldGameObject)->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(gameObject()->position(), newGameObject->rotation(),
                                        newGameObject->scale());

            dynamic_cast<Internal::GameObject*>(newGameObject)->registerObserver(this);
        }
    }

    void SpriteComponent::filepath(const std::string &path) {
        if(path.empty()){
            if(graphic_)
                Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);

            graphicLoaded_.reset();
            graphic_.reset();
        }
        else{
            graphicLoaded_ = std::make_shared<Internal::Graphic>(path);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            Internal::GraphicsEngine::GetInstance().registerGraphic(graphic_);
        }
        filePath_ = path;
    }

    std::string SpriteComponent::filepath() const {
        return filePath_;
    }

    geComponentRef SpriteComponent::instantiate() const {
        return std::make_shared<SpriteComponent>();
    }
}