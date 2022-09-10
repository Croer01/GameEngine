//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include <game-engine/internal/graphics/Graphic.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {

class PUBLICAPI SpriteComponentData : public ComponentData
{
public:
    SpriteComponentData()
    {
        createPropertyFilePath("filePath", "",FileType::IMAGE, true);
        createProperty<geColor>("tint", geColor(1.f));
        createProperty<bool>("visible", true);
        createProperty<Vec2D>("displacement", Vec2D());
        createProperty<bool>("repeat", false);
        createPropertyEnum("anchor", "TOP_LEFT", {
            "TOP_LEFT",
            "TOP_CENTER",
            "TOP_RIGHT",
            "MIDDLE_LEFT",
            "MIDDLE_CENTER",
            "MIDDLE_RIGHT",
            "BOTTOM_LEFT",
            "BOTTOM_CENTER",
            "BOTTOM_RIGHT"
        });
    }
};

    class PUBLICAPI SpriteComponent
            : public geComponentInstantiable<SpriteComponent, SpriteComponentData>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;

        Internal::GraphicsEngine *graphicsEngine_;

        void updateGraphicRef();
        void updateVisible();
        void updateTint();
        void updateAnchor();
        void updateDisplacement();

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~SpriteComponent();

        virtual void preInit();

        int getWidth() const;

        int getHeight() const;

        void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;
    };
}

#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
