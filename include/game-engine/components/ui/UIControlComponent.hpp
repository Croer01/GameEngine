//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
#define GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/properties/PropertiesManager.hpp>

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
        void Update(float elapsedTime) override;
        virtual ~UIControlComponent() = default;
        void screenPos(const Vec2D &pos);
        Vec2D screenPos() const;

        void screenSize(const Vec2D &size);
        Vec2D screenSize() const;

        bool isFocused() const;
    };

PROPERTIES(UIControlComponentProperties)
class UIControlComponentProperties : public PropertyInstantiator
{
public:
    virtual PropertySetBase* instantiateProperties()
    {
        auto *properties = new PropertySet<UIControlComponent>();

        properties->add(new Property<UIControlComponent, Vec2D>(
            "screenPos",
            &UIControlComponent::screenPos,
            &UIControlComponent::screenPos,
            Vec2D(),
            true
        ));

        properties->add(new Property<UIControlComponent, Vec2D>(
            "screenSize",
            &UIControlComponent::screenSize,
            &UIControlComponent::screenSize,
            Vec2D(),
            true
        ));
        return properties;
    }
};
}

#endif //GAMEENGINEEDITOR_UICONTROLCOMPONENT_HPP
