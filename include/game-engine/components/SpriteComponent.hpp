//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include "../private/graphics/Graphic.hpp"
#include "../private/graphics/GraphicHolder.hpp"
#include <game-engine/properties/PropertiesRegister.hpp>

namespace GameEngine {

    class PUBLICAPI SpriteComponent
            : public geComponentInstantiable<SpriteComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        bool visible_;
        std::string filePath_;
        std::string anchor_;

        void updateGraphicRef();

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~SpriteComponent();

        void init() override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);

        std::string filepath() const;

        void anchor(const std::string &anchor);

        std::string anchor() const;
    };

PROPERTIES(SpriteComponentProperties)
class SpriteComponentProperties : public PropertyInstantiator
{
public:
    virtual PropertySetBase* instantiateProperties()
    {
        auto *properties = new PropertySet<SpriteComponent>();

        properties->add(new Property<SpriteComponent, std::string>(
            "filePath",
            &SpriteComponent::filepath,
            &SpriteComponent::filepath,
            "",
            true));
        properties->add(new Property<SpriteComponent, bool>(
            "visible",
            &SpriteComponent::isVisible,
            &SpriteComponent::setVisible,
            true));

        properties->add(new Property<SpriteComponent, std::string>(
            "anchor",
            &SpriteComponent::anchor,
            &SpriteComponent::anchor,
            "",
            false));
        return properties;
    }
};
}

#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
