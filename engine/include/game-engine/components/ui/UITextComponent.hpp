//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP


#include "UIControlComponent.hpp"
#include "../../../../private/graphics/font/Text.hpp"
#include <game-engine/properties/PropertiesManager.hpp>

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

    std::string getPropertiesName() const override;

    void init() override;

    int fontSize() const;
    void fontSize(const int &size);

    std::string font() const;
    void font(const std::string &fontName);

    void text(const std::string &value);
    std::string text() const;

    Vec2D getTextSize() const;
};

PROPERTIES(UITextComponentProperties)
class UITextComponentProperties : public PropertyInstantiator
{
public:
    virtual PropertySetBase *instantiateProperties()
    {
        std::shared_ptr<PropertySetBase> base = PropertiesManager::GetInstance().instantiate("UIControlComponentProperties");
        auto *properties = new PropertySet<UITextComponent>(base.get());

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
