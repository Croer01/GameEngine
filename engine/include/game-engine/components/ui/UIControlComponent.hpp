//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINE_UICONTROLCOMPONENT_HPP
#define GAMEENGINE_UICONTROLCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>

namespace GameEngine {

class UIControlComponentData : public ComponentData
{
public:
    UIControlComponentData()
    {
        createProperty<std::string>("id", "", true);
        createProperty<Vec2D>("screenPos", Vec2D(), true);
        createProperty<Vec2D>("screenSize", Vec2D(), true);
        createProperty<bool>("visible", true);
    }
};

    class PUBLICAPI UIControlComponent : public geComponentInstantiable<UIControlComponent, UIControlComponentData>
    {
        bool hover_;
        bool focused_;
        Internal::GraphicsEngine *graphicsEngine_;

    protected:
        virtual void onClick() {};
        virtual void onHoverIn() {};
        virtual void onHoverOut() {};
        virtual void onFocusChanged() {};
        virtual void onVisibleChanged() {};
        Internal::GraphicsEngine *graphicsEngine();

        Vec2D calculateVirtualScreenPos() const;
        Vec2D calculateVirtualScreenSize() const;
    public:
        virtual ~UIControlComponent() = default;

        virtual void preInit();
        void Update(float elapsedTime) override;

        bool isFocused() const;
    };
}

#endif //GAMEENGINE_UICONTROLCOMPONENT_HPP
