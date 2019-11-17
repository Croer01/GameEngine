//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP

#include <game-engine/Command.hpp>
#include "UITextComponent.hpp"
#include "../../../../private/graphics/GraphicHolder.hpp"
#include <game-engine/properties/PropertiesRegister.hpp>

namespace GameEngine {
class PUBLICAPI UIButtonComponent : public UITextComponent
{
    CommandRef command_;
    std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;
    geColor background_;
    geColor hoverBackground_;

    void changeColor(bool isHover);
protected:
    geComponentRef instantiate() const override;
    void createBackgroundGraphic();
    void onClick() override;
    void onHoverIn() override;
    void onHoverOut() override;
public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
    void init() override;
    void setCommand(const CommandRef &command);

    void background(const geColor &color);
    geColor background() const;

    void hoverBackground(const geColor &color);
    geColor hoverBackground() const;
};

PROPERTIES(UIButtonComponentProperties)
class UIButtonComponentProperties : public PropertyInstantiator
{
public:
    virtual PropertySetBase *instantiatePropertie()
    {
        std::shared_ptr<PropertySetBase> base = PropertiesRegister::GetInstance().instantiate("UITextComponentProperties");
        auto *properties = new PropertySet<UIButtonComponent>(base.get());

        properties->add(new Property<UIButtonComponent, geColor>(
            "background",
            &UIButtonComponent::background,
            &UIButtonComponent::background,
            geColor(1.f)
        ));

        properties->add(new Property<UIButtonComponent, geColor>(
            "hoverBackground",
            &UIButtonComponent::hoverBackground,
            &UIButtonComponent::hoverBackground,
            geColor(.8f)
        ));

        return properties;
    }
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
