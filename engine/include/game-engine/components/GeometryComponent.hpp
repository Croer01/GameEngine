//
// Created by adria on 16/04/2019.
//

#ifndef ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
#define ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP

#include <vector>
#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/internal/graphics/GraphicGeometry.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {
class PUBLICAPI GeometryColliderComponentData : public ComponentData
{
public:
    GeometryColliderComponentData()
    {
        createProperty<std::vector<Vec2D>>("path", {}, true);
        createProperty<bool>("visible",true);
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
        createProperty<geColor>("tint",geColor(1.f));
        createProperty<bool>("fill", true);
    }
};

    class PUBLICAPI GeometryComponent : public geComponentInstantiable<GeometryComponent, GeometryColliderComponentData>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::GraphicGeometry> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        Internal::GraphicsEngine *graphicsEngine_;

        void updateGraphicRef();
        void updateVisible();
        void updateTint();
        void updateFill();
        void updateAnchor();

    protected:
        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~GeometryComponent();

        virtual void preInit();

        void init() override;

        int getWidth() const;

        int getHeight() const;

        void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;
    };
}


#endif //ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
