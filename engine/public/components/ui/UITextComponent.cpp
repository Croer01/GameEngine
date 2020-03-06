//
// Created by adria on 11/09/2019.
//

#include "game-engine/components/ui/UITextComponent.hpp"
#include "../../../private/graphics/font/FontManager.hpp"
#include "../../../private/graphics/GraphicsEngine.hpp"

namespace GameEngine
{
UITextComponent::~UITextComponent()
{
    if(graphicText_)
        Internal::GraphicsEngine::GetInstance().unregisterText(graphicText_);
}

geComponentRef UITextComponent::instantiate() const
{
    return std::make_shared<UITextComponent>();
}

geComponentRef UITextComponent::clone() const
{
    geComponentRef cloned = instantiate();
    auto compCloned = std::dynamic_pointer_cast<UITextComponent>(cloned);
    auto other = shared_from_this();
    auto thisRef = std::dynamic_pointer_cast<const UITextComponent>(other);
    compCloned->copyProperties<UITextComponent>(thisRef);

    return cloned;
}

void UITextComponent::init()
{
    UpdateTextGraphic();
}

void UITextComponent::text(const std::string &value)
{
    text_ = value;
    if(graphicText_)
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
    if(graphicText_)
        UpdateTextGraphic();
}

std::string UITextComponent::font() const
{
    return font_;
}

void UITextComponent::font(const std::string &fontName)
{
    font_ = fontName;
    if(graphicText_)
        UpdateTextGraphic();
}

void UITextComponent::UpdateTextGraphic()
{
    if(graphicText_)
        Internal::GraphicsEngine::GetInstance().unregisterText(graphicText_);

    if(!font_.empty())
    {
        graphicText_ = Internal::FontManager::GetInstance().getFont(font_, fontSize_)->createText(text_);
        setTextModelTransform(calculateVirtualScreenPos(), Vec2D(), Vec2D(1, 1));
        graphicText_->setTintColor(foregroundColor_);
        Internal::GraphicsEngine::GetInstance().registerText(graphicText_);
        graphicText_->setActive(visible());
    }
}

void UITextComponent::setTextModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale)
{
    graphicText_->setModelTransform(position, rotation, scale);
}

Vec2D UITextComponent::getTextSize() const
{
    return Vec2D(graphicText_->width(), graphicText_->height());
}

std::shared_ptr<Internal::Text> UITextComponent::getGraphicText() const
{
    return graphicText_;
}

void UITextComponent::foregroundColor(const geColor &value)
{
    foregroundColor_ = value;
    if(graphicText_)
        UpdateTextGraphic();
}
geColor UITextComponent::foregroundColor() const
{
    return foregroundColor_;
}

PropertySetBase *UITextComponent::getProperties() const
{
    PropertySetBase *base = UIControlComponent::getProperties();
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

    properties->add(new Property<UITextComponent, geColor>(
            "foreground",
            &UITextComponent::foregroundColor,
            &UITextComponent::foregroundColor,
            geColor(0)));

    return properties;

}

void UITextComponent::onVisibleChanged()
{
    if(graphicText_)
        graphicText_->setActive(visible());
}
}
