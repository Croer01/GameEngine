//
// Created by adria on 16/04/2019.
//

#include <game-engine/components/GeometryComponent.hpp>
#include "../../private/graphics/GraphicsEngine.hpp"

namespace GameEngine {
    void GeometryComponent::init() {
        updateGraphicRef();
        visible_ = true; //TODO: remove this line after set properties
        setVisible(visible_);
        graphic_->setAnchor(Internal::GraphicAnchor::TOP_LEFT);
        graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
        gameObject()->registerObserver(this);
    }

    void GeometryComponent::updateGraphicRef() {
        if (graphic_)
            Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);

        std::vector<Vec2D> path = std::vector<Vec2D>({
                                                             Vec2D(3, 0),
                                                             Vec2D(6, 0),
                                                             Vec2D(9, 4.5f),
                                                             Vec2D(6, 9),
                                                             Vec2D(3, 9),
                                                             Vec2D(0, 4.5f)
                                                     });

        graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
        graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
        Internal::GraphicsEngine::GetInstance().registerGraphic(graphic_);
    }

    void GeometryComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(newGameObject->position(), newGameObject->rotation(),
                                        newGameObject->scale());

            newGameObject->registerObserver(this);
        }
    }

    int GeometryComponent::getWidth() const {
        return graphicLoaded_->getWidth();
    }

    int GeometryComponent::getHeight() const {
        return graphicLoaded_->getHeight();
    }

    void GeometryComponent::onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) {
        if (event == GameObjectEvent::TransformChanged) {
            graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(),
                                        gameObject()->scale());
        } else if (event == GameObjectEvent::ActiveChanged) {
            graphic_->setActive(gameObject()->active() && visible_);
        }
    }

    GeometryComponent::~GeometryComponent() {
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
    }

    void GeometryComponent::setVisible(const bool &visible) {
        visible_ = visible;
        if(graphic_)
            graphic_->setActive(gameObject() && gameObject()->active() && visible_);
    }

    bool GeometryComponent::isVisible() const {
        return visible_;
    }
}

