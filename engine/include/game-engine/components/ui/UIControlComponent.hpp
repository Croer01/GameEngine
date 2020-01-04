//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
#define GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP


#include <game-engine/geComponent.hpp>

namespace GameEngine {
    class PUBLICAPI UIControlComponent : public geComponentInstantiable<UIControlComponent>
    {
        Vec2D screenPos_;
        Vec2D screenSize_;
        bool hover_;
        bool focused_;

    protected:
        virtual void onClick() {};
        virtual void onHoverIn() {};
        virtual void onHoverOut() {};
        virtual void onFocusChanged() {};
    public:
        virtual ~UIControlComponent() = default;
        PropertySetBase *getProperties() const override;

        void Update(float elapsedTime) override;
        void screenPos(const Vec2D &pos);
        Vec2D screenPos() const;

        void screenSize(const Vec2D &size);
        Vec2D screenSize() const;

        bool isFocused() const;
    };
}

#endif //GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP