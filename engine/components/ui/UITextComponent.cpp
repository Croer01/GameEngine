//
// Created by adria on 11/09/2019.
//

#include <game-engine/components/ui/UITextComponent.hpp>
#include <game-engine/internal/graphics/font/FontManager.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/Game.hpp>

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
    cloned->setData(getData()->template clone<UITextComponentData>());
    return cloned;
}

void UITextComponent::init()
{
    setPropertyObserver<std::string>("text", [this](){ UpdateTextGraphic();});
    setPropertyObserver<std::string>("font", [this](){ UpdateTextGraphic();});
    setPropertyObserver<int>("fontSize", [this](){ UpdateTextGraphic();});
    setPropertyObserver<geColor>("foregroundColor", [this](){ UpdateTextGraphic();});
    UpdateTextGraphic();
}

void UITextComponent::UpdateTextGraphic()
{
    if(!graphicsEngine())
        return;

    if(graphicText_)
        graphicsEngine()->unregisterText(graphicText_);

    Game *game = gameObject()->game();
    std::string text = getPropertyValue<std::string>("text");
    std::string font = getPropertyValue<std::string>("font");
    int fontSize = getPropertyValue<int>("fontSize");
    geColor foregroundColor = getPropertyValue<geColor>("foregroundColor");
    if(!font.empty())
        graphicText_ = game->fontManager()->getFont(font, fontSize)->createText(text);
    else
        graphicText_ = game->fontManager()->getDefaultFont(fontSize)->createText(text);

    setTextModelTransform(calculateVirtualScreenPos(), 0.f, Vec2D(1, 1));
    graphicText_->setTintColor(foregroundColor);
    graphicsEngine()->registerText(graphicText_);
    graphicText_->setActive(getPropertyValue<bool>("visible"));
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

void UITextComponent::onVisibleChanged()
{
    if(graphicText_)
        graphicText_->setActive(getPropertyValue<bool>("visible"));
}

ComponentDataRef UITextComponent::instantiateData() const
{
    return std::make_shared<UITextComponentData>();
}
}
