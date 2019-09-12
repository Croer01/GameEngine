//
// Created by adria on 11/09/2019.
//

#include "game-engine/components/ui/UITextComponent.hpp"
#include "../../../private/graphics/font/FontManager.hpp"
#include "../../../private/graphics/GraphicsEngine.hpp"

namespace GameEngine
{
geComponentRef UITextComponent::instantiate() const
{
    return std::make_shared<UITextComponent>();
}

void UITextComponent::init()
{
    UpdateTextGraphic();
}

PropertySetBase *UITextComponent::configureProperties()
{
    PropertySetBase *base = UIControlComponent::configureProperties();
    auto *properties = new PropertySet<UITextComponent>(this, base);

    properties->add(new Property<UITextComponent, std::string>(
        "text",
        this,
        &UITextComponent::text,
        &UITextComponent::text,
        "",
        true
        ));

    properties->add(new Property<UITextComponent, std::string>(
        "font",
        this,
        &UITextComponent::font,
        &UITextComponent::font,
        "",
        true));

    properties->add(new Property<UITextComponent, int>(
        "fontSize",
        this,
        &UITextComponent::fontSize,
        &UITextComponent::fontSize,
        0));

    return properties;
}

void UITextComponent::text(const std::string &value)
{
    text_ = value;
    UpdateTextGraphic();
}

std::string UITextComponent::text() const
{
    return text_;
}

int UITextComponent::fontSize() const
{
    return fontSize_;
}

void UITextComponent::fontSize(const int &size)
{
    fontSize_ = size;
    UpdateTextGraphic();
}

std::string UITextComponent::font() const
{
    return font_;
}

void UITextComponent::font(const std::string &fontName)
{
    font_ = fontName;
    UpdateTextGraphic();
}

void UITextComponent::UpdateTextGraphic()
{
    if(graphicText_)
        Internal::GraphicsEngine::GetInstance().unregisterText(graphicText_);

    if(!font_.empty())
    {
        graphicText_ = Internal::FontManager::GetInstance().getFont(font_, fontSize_)->createText(text_);
        graphicText_->setModelTransform(screenPos(), Vec2D(), Vec2D(1, 1));
        Internal::GraphicsEngine::GetInstance().registerText(graphicText_);
    }
}
}
