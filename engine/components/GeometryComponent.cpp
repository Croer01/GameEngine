//
// Created by adria on 16/04/2019.
//

#include <game-engine/components/GeometryComponent.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
    void GeometryComponent::preInit()
    {
        graphicsEngine_ = gameObject()->game()->graphicsEngine();
        //TODO: implement Layers or improve how to register sprites to not have this
        // "unordered" graphic issues

        // updateGraphicRef();
    }

    void GeometryComponent::init() {
        setPropertyObserver<bool>("visible",[this](){ updateVisible(); });
        setPropertyObserver<std::vector<Vec2D>>("path", [this](){ updateGraphicRef(); });
        setEnumPropertyObserver("anchor", [this](){ updateAnchor(); });
        setPropertyObserver<geColor>("tint", [this](){ updateTint(); });
        setPropertyObserver<bool>("fill", [this](){ updateFill(); });

        //TODO: implement Layers or improve how to register sprites
        updateGraphicRef();
        // the preInit ensure that the graphic is already created at this point
        gameObject()->registerObserver(this);
    }

    void GeometryComponent::updateGraphicRef() {
        if(gameObject() == nullptr || gameObject()->game() == nullptr)
            return;

        if (graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);


        auto path = getPropertyValue<std::vector<Vec2D>>("path");

        if(path.empty()){
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            graphicsEngine_->registerGraphic(graphic_);

            updateVisible();
            updateAnchor();
            updateTint();
            updateFill();
            graphic_->setModelTransform(gameObject());
        }
    }

    void GeometryComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(newGameObject);

            newGameObject->registerObserver(this);
        }
    }

    int GeometryComponent::getWidth() const {
        return graphicLoaded_->getWidth();
    }

    int GeometryComponent::getHeight() const {
        return graphicLoaded_->getHeight();
    }

    void GeometryComponent::onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) {
        if (event == GameObjectEvent::TransformChanged) {
            graphic_->setModelTransform(gameObject());
        } else if (event == GameObjectEvent::ActiveChanged) {
            bool visible = getPropertyValue<bool>("visible");
            graphic_->setActive(gameObject()->active() && visible);
        }
    }

    GeometryComponent::~GeometryComponent() {
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
    }

    void GeometryComponent::updateVisible()
    {
        if(graphic_){
            bool visible = getPropertyValue<bool>("visible");
            graphic_->setActive(gameObject() && gameObject()->active() && visible);
        }
    }

    void GeometryComponent::updateTint()
    {
        if(graphic_){
            geColor tint = getPropertyValue<geColor>("tint");
            graphic_->setTintColor(tint);
        }
    }

    void GeometryComponent::updateFill()
    {
        if(graphic_){
            bool fill = getPropertyValue<bool>("fill");
            graphicLoaded_->drawFillGeometry(fill);
        }
    }

void GeometryComponent::updateAnchor() {
    if(graphic_)
    {
        std::string anchor = getEnumPropertyValue("anchor");
        GameEngine::Internal::GraphicAnchor graphicAnchor = Internal::parseStringToGraphicAnchor(anchor);
        graphic_->setAnchor(graphicAnchor);
    }
}

}

