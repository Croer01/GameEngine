//
// Created by adria on 16/04/2019.
//

#ifndef ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
#define ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP

#include <vector>
#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include "../../../private/graphics/GraphicGeometry.hpp"
#include "../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
    class PUBLICAPI GeometryComponent : public geComponentInstantiable<GeometryComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        bool visible_;
        void updateGraphicRef();

    protected:
        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    public:
        virtual ~GeometryComponent();

        void init() override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
    };
}


#endif //ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
