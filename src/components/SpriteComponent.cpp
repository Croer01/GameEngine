//
// Created by adria on 08/10/2018.
//

#include "SpriteComponent.hpp"
#include "../graphics/GraphicsEngine.hpp"

namespace GameEngine {
namespace Internal {
    void SpriteComponent::init() {
        setVisible(visible_);
    }

    std::shared_ptr<Component> SpriteComponent::Clone() {
        std::shared_ptr<SpriteComponent> clone = std::make_shared<SpriteComponent>();
        clone->graphicLoaded_ = graphicLoaded_;
        clone->graphic_ = std::make_shared<GraphicHolder>(graphicLoaded_);
        GraphicsEngine::GetInstance().registerGraphic(clone->graphic_);
        clone->visible_ = visible_;
        return clone;
    }

    void SpriteComponent::fromFile(const YAML::Node &componentConfig) {
        if (!componentConfig["filePath"])
            throw std::logic_error("property filePath not defined");
        graphicLoaded_ = std::make_shared<Graphic>(componentConfig["filePath"].as<std::string>());
        visible_ = componentConfig["visible"].as<bool>(true);
    }

    int SpriteComponent::getWidth() const {
        return graphicLoaded_->getWidth();
    }

    int SpriteComponent::getHeight() const {
        return graphicLoaded_->getHeight();
    }

    void SpriteComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if (event == GameObjectEvent::TransformChanged) {
            graphic_->setModelTransform(gameObject()->getPosition(), gameObject()->getRotation(),
                                        gameObject()->getScale());
        } else if (event == GameObjectEvent::ActiveChanged) {
            graphic_->setActive(gameObject()->isActive() && visible_);
        }
    }

    SpriteComponent::~SpriteComponent() {
        GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
    }

    void SpriteComponent::setVisible(bool visible) {
        visible_ = visible;
        graphic_->setActive(gameObject()->isActive() && visible_);
    }

    bool SpriteComponent::isVisible() const {
        return visible_;
    }

    void SpriteComponent::onGameObjectChange(GameObject *oldGameObject, GameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(gameObject()->getPosition(), newGameObject->getRotation(),
                                        newGameObject->getScale());

            newGameObject->registerObserver(this);
        }
    }
}
}