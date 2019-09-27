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

    protected:
        PropertySetBase *configureProperties() override;
        virtual void onClick() {};
        virtual void onHoverIn() {};
        virtual void onHoverOut() {};
    public:
        void Update(float elapsedTime) override;
        virtual ~UIControlComponent() = default;
        void screenPos(const Vec2D &pos);
        Vec2D screenPos() const;

        void screenSize(const Vec2D &size);
        Vec2D screenSize() const;
    };

}

#endif //GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
