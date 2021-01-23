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

namespace GameEngine {
    class PUBLICAPI GeometryComponent : public geComponentInstantiable<GeometryComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::GraphicGeometry> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        Internal::GraphicsEngine *graphicsEngine_;
        std::vector<Vec2D> path_;
        std::string anchor_;
        geColor color_;
        bool visible_;
        bool fill_;
        void updateGraphicRef();

    protected:
        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~GeometryComponent();

        PropertySetBase *getProperties() const override;

        virtual void preInit();

        void init() override;

        int getWidth() const;

        int getHeight() const;

        void visible(const bool &visible);

        bool visible() const;

        void fill(const bool &value);

        bool fill() const;

        void path (const std::vector<Vec2D> &pathArray);
        std::vector<Vec2D> path () const;

        void anchor(const std::string &anchor);
        std::string anchor() const;

        void color(const geColor &value);
        geColor color() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
    };
}


#endif //ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
