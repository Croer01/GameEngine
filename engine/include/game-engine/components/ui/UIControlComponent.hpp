//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
#define GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include "../../../../private/graphics/GraphicsEngine.hpp"

namespace GameEngine {
    class PUBLICAPI UIControlComponent : public geComponentInstantiable<UIControlComponent>
    {
        std::string id_;
        Vec2D screenPos_;
        Vec2D screenSize_;
        bool hover_;
        bool focused_;
        bool visible_;
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
        PropertySetBase *getProperties() const override;

        virtual void preInit();
        void Update(float elapsedTime) override;
        void screenPos(const Vec2D &pos);
        Vec2D screenPos() const;

        void screenSize(const Vec2D &size);
        Vec2D screenSize() const;

        void id(const std::string &value);
        std::string id() const;

        bool visible() const;
        void visible(const bool &value);

        bool isFocused() const;
    };
}

#endif //GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
