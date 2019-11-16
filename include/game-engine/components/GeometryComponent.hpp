//
// Created by adria on 16/04/2019.
//

#ifndef ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
#define ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP

#include <vector>
#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/properties/PropertiesRegister.hpp>
#include "../../../private/graphics/GraphicGeometry.hpp"
#include "../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
    class PUBLICAPI GeometryComponent : public geComponentInstantiable<GeometryComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        std::vector<Vec2D> path_;
        std::string anchor_;
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

        void visible(const bool &visible);

        bool visible() const;

        void path (const std::vector<Vec2D> &pathArray);
        std::vector<Vec2D> path () const;

        void anchor(const std::string &anchor);

        std::string anchor() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
    };

template <>
class PropertyInstantiator<GeometryComponent>
{
public:
    static std::string getTargetName()
    {
        return "GeometryComponent";
    }

    static PropertySetBase* instantiate()
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

        properties->add(new Property<GeometryComponent, std::string>(
            "anchor",
            &GeometryComponent::anchor,
            &GeometryComponent::anchor,
            "",
            false));
        return properties;
    }
};
}


#endif //ZOMBIEWAVE_GEOMETRYCOMPONENT_HPP
