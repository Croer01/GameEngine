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
        graphicsEngine_ = gameObject()->game()->graphicsEngine();

        //TODO: implement Layers or improve how to register sprites
        updateGraphicRef();
        // the preInit ensure that the graphic is already created at this point
        setVisible(visible_);
        anchor(anchor_);
        displacement(displacement_);
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
            graphic_->setActive(gameObject()->active() && visible_);
        }
    }

    SpriteComponent::~SpriteComponent() {
        if(graphicsEngine_ != nullptr && graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);
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
            graphic_->setModelTransform(newGameObject);

            newGameObject->registerObserver(this);
        }
    }

    void SpriteComponent::filepath(const std::string &path) {
        filePath_ = path;
        updateGraphicRef();
    }

    std::string SpriteComponent::filepath() const {
        return filePath_;
    }

    void SpriteComponent::updateGraphicRef() {
        if(gameObject() == nullptr || gameObject()->game() == nullptr)
            return;

        if(graphic_)
            graphicsEngine_->unregisterGraphic(graphic_);

        if(filePath_.empty()){
            graphicLoaded_.reset();
            graphic_.reset();
        } else {
            graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath_);
            graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
            graphicsEngine_->registerGraphic(graphic_);
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

    Vec2D SpriteComponent::displacement() const
    {
        return displacement_;
    }

    void SpriteComponent::displacement(const Vec2D &value)
    {
        displacement_ = value;
        if(graphic_)
            graphic_->setDisplacement(displacement_);
    }

PropertySetBase *SpriteComponent::getProperties() const
{
    auto *properties = new PropertySet<SpriteComponent>();

    properties->add(new PropertyFilePath<SpriteComponent>(
            "filePath",
            &SpriteComponent::filepath,
            &SpriteComponent::filepath,
            "",
            FileType::IMAGE,
            true));
    properties->add(new Property<SpriteComponent, bool>(
            "visible",
            &SpriteComponent::isVisible,
            &SpriteComponent::setVisible,
            true));
    properties->add(new Property<SpriteComponent, Vec2D>(
        "displacement",
        &SpriteComponent::displacement,
        &SpriteComponent::displacement,
        Vec2D()));

    properties->add(new PropertyEnum<SpriteComponent>(
            "anchor",
            &SpriteComponent::anchor,
            &SpriteComponent::anchor,
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

    return properties;

}
}
