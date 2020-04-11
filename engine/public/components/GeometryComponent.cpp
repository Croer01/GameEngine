//
// Created by adria on 16/04/2019.
//

#include <game-engine/components/GeometryComponent.hpp>
#include "../../private/graphics/GraphicsEngine.hpp"
#include "../../private/Game.hpp"

namespace GameEngine {
    void GeometryComponent::preInit()
    {
        graphicsEngine_ = dynamic_cast<Internal::Game*>(gameObject()->game())->graphicsEngine();
        updateGraphicRef();
    }

    void GeometryComponent::init() {
        // the preInit ensure that the graphic is already created at this point
        visible(visible_);
        anchor(anchor_);
        color(color_);
        graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
        gameObject()->registerObserver(this);
    }

    void GeometryComponent::updateGraphicRef() {
        if(gameObject() == nullptr || gameObject()->game() == nullptr)
            return;

        if (graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);

        if(path_.empty()){
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path_);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            graphicsEngine_->registerGraphic(graphic_);
        }
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
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
    }

    void GeometryComponent::visible(const bool &visible) {
        visible_ = visible;
        if(graphic_)
            graphic_->setActive(gameObject() && gameObject()->active() && visible_);
    }

    bool GeometryComponent::visible() const {
        return visible_;
    }

    void GeometryComponent::path(const std::vector<Vec2D> &pathArray) {
        path_ = pathArray;
    }

    std::vector<Vec2D> GeometryComponent::path() const {
        return path_;
    }

    std::string GeometryComponent::anchor() const {
        return anchor_;
    }

    void GeometryComponent::anchor(const std::string &anchor) {

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

PropertySetBase *GeometryComponent::getProperties() const
{
    auto *properties = new PropertySet<GeometryComponent>();

    properties->add(new Property<GeometryComponent, std::vector<Vec2D>>(
            "path",
            &GeometryComponent::path,
            &GeometryComponent::path,
            {},
            true));
    properties->add(new Property<GeometryComponent, bool>(
            "visible",
            &GeometryComponent::visible,
            &GeometryComponent::visible,
            true));

    properties->add(new PropertyEnum<GeometryComponent>(
            "anchor",
            &GeometryComponent::anchor,
            &GeometryComponent::anchor,
            "TOP_LEFT",
            {
                    "TOP_LEFT",
                    "TOP_CENTER",
                    "TOP_RIGHT",
                    "MIDDLE_LEFT",
                    "MIDDLE_CENTER",
                    "MIDDLE_RIGHT",
                    "BOTTOM_LEFT",
                    "BOTTOM_CENTER",
                    "BOTTOM_RIGHT"
            }));

    properties->add(new Property<GeometryComponent, geColor>(
            "tint",
            &GeometryComponent::color,
            &GeometryComponent::color,
            geColor(1.f)));
    return properties;

}

void GeometryComponent::color(const geColor &value)
{
    color_ = value;
    if(graphic_)
        graphic_->setTintColor(color_);
}

geColor GeometryComponent::color() const
{
    return color_;
}
}

