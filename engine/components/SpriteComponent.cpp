//
// Created by adria on 08/10/2018.
//

#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {

    void SpriteComponent::preInit()
    {
        setFilePathPropertyObserver("filePath", [this](){ updateGraphicRef(); });
        setPropertyObserver<geColor>("tint", [this](){ updateTint(); });
        setPropertyObserver<bool>("visible", [this](){ updateVisible(); });
        setPropertyObserver<Vec2D>("displacement", [this](){ updateDisplacement(); });
        setEnumPropertyObserver("anchor", [this](){ updateAnchor(); });
        graphicsEngine_ = gameObject()->game()->graphicsEngine();

        //TODO: implement Layers or improve how to register sprites
        updateGraphicRef();
        // the preInit ensure that the graphic is already created at this point
        updateTint();
        updateVisible();
        updateAnchor();
        updateDisplacement();
        graphic_->setModelTransform(gameObject());
        gameObject()->registerObserver(this);
    }

    int SpriteComponent::getWidth() const {
        return graphicLoaded_->getWidth();
    }

    int SpriteComponent::getHeight() const {
        return graphicLoaded_->getHeight();
    }

    void SpriteComponent::onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) {
        if (event == GameObjectEvent::TransformChanged) {
            graphic_->setModelTransform(gameObject());
        } else if (event == GameObjectEvent::ActiveChanged) {
            updateVisible();
        }
    }

    SpriteComponent::~SpriteComponent() {
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
    }

    void SpriteComponent::onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {
        if (oldGameObject)
            oldGameObject->unregisterObserver(this);

        if (newGameObject && graphic_) {
            graphic_->setModelTransform(newGameObject);

            newGameObject->registerObserver(this);
        }
    }

    void SpriteComponent::updateGraphicRef() {
        if(gameObject() == nullptr || gameObject()->game() == nullptr)
            return;

        if(graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);

        const std::string &filePath = getFilePathPropertyValue("filePath");

        if(filePath.empty()){
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            graphicsEngine_->registerGraphic(graphic_);
        }
    }

void SpriteComponent::updateVisible()
{
    if(graphic_){
        bool visible = getPropertyValue<bool>("visible");
        graphic_->setActive(gameObject() && gameObject()->active() && visible);
    }
}

void SpriteComponent::updateTint()
{
    if(graphic_){
        geColor tint = getPropertyValue<geColor>("tint");
        graphic_->setTintColor(tint);
    }
}

void SpriteComponent::updateAnchor() {
    if(graphic_)
    {
        std::string anchor = getEnumPropertyValue("anchor");
        GameEngine::Internal::GraphicAnchor graphicAnchor = Internal::parseStringToGraphicAnchor(anchor);
        graphic_->setAnchor(graphicAnchor);
    }
}

void SpriteComponent::updateDisplacement() {
    if(graphic_)
    {
        const Vec2D &displacement = getPropertyValue<Vec2D>("displacement");
        graphic_->setDisplacement(displacement);
    }
}
}
