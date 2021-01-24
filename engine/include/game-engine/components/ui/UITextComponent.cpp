//
// Created by adria on 11/09/2019.
//

#include "game-engine/components/ui/UITextComponent.hpp"
#include <game-engine/internal/graphics/font/FontManager.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include "game-engine/Game.hpp"

namespace GameEngine
{
UITextComponent::~UITextComponent()
{
    if(graphicText_)
        graphicsEngine()->unregisterText(graphicText_);
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
    if(!graphicsEngine())
        return;

    if(graphicText_)
        graphicsEngine()->unregisterText(graphicText_);

    Game *game = gameObject()->game();
    if(!font_.empty())
        graphicText_ = game->fontManager()->getFont(font_, fontSize_)->createText(text_);
    else
        graphicText_ = game->fontManager()->getDefaultFont(fontSize_)->createText(text_);

    setTextModelTransform(calculateVirtualScreenPos(), 0.f, Vec2D(1, 1));
    graphicText_->setTintColor(foregroundColor_);
    graphicsEngine()->registerText(graphicText_);
    graphicText_->setActive(visible());
}

void UITextComponent::setTextModelTransform(const Vec2D &position, float rotation, const Vec2D &scale)
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
