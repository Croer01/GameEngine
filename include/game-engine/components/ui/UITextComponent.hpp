//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP


#include "UIControlComponent.hpp"
#include "../../../../private/graphics/font/Text.hpp"
#include <game-engine/properties/PropertiesRegister.hpp>

namespace GameEngine {
class PUBLICAPI UITextComponent : public UIControlComponent
{
    std::shared_ptr<Internal::Text> graphicText_;
    std::string text_;
    std::string font_;
    int fontSize_;

    void UpdateTextGraphic();
protected:
    geComponentRef instantiate() const override;
    void setTextModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale);
    std::shared_ptr<Internal::Text> getGraphicText() const;
public:
    virtual ~UITextComponent();
    void init() override;

    int fontSize() const;
    void fontSize(const int &size);

    std::string font() const;
    void font(const std::string &fontName);

    void text(const std::string &value);
    std::string text() const;

    Vec2D getTextSize() const;
};

template <>
class PropertyInstantiator<UITextComponent>
{
public:
    static std::string getTargetName()
    {
        return "UITextComponent";
    }
    static PropertySetBase* instantiate()
    {
        PropertySetBase *base = PropertyInstantiator<UIControlComponent>::instantiate();
        auto *properties = new PropertySet<UITextComponent>(base);

        properties->add(new Property<UITextComponent, std::string>(
            "text",
            &UITextComponent::text,
            &UITextComponent::text,
            "",
            true
        ));

        properties->add(new Property<UITextComponent, std::string>(
            "font",
            &UITextComponent::font,
            &UITextComponent::font,
            "",
            true));

        properties->add(new Property<UITextComponent, int>(
            "fontSize",
            &UITextComponent::fontSize,
            &UITextComponent::fontSize,
            0));

        return properties;
    }
};
}


#endif //GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
