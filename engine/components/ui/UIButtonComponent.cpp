//
// Created by adria on 23/08/2019.
//

#include <game-engine/InputManager.hpp>
#include <iostream>
#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/GeometryComponent.hpp>
#include <game-engine/components/ui/UIButtonComponent.hpp>
#include <game-engine/internal/graphics/font/FontManager.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
UIButtonComponent::UIButtonComponent()
{}

void UIButtonComponent::init()
{
    UITextComponent::init();
    createBackgroundGraphic();
}

void UIButtonComponent::changeColor(bool isHover)
{
    createBackgroundGraphic();
    if(backgroundGraphic_)
    {
        geColor background = getPropertyValue<geColor>("background");
        geColor hoverBackground = getPropertyValue<geColor>("hoverBackground");
        backgroundGraphic_->setTintColor(isHover ? hoverBackground : background);
    }
}

void UIButtonComponent::setCommand(const CommandRef &command)
{
    command_ = command;
}

UIButtonComponent::~UIButtonComponent()
{
    if(backgroundGraphic_)
        graphicsEngine()->unregisterGraphic(backgroundGraphic_);
}

geComponentRef UIButtonComponent::instantiate() const
{
    return std::make_shared<UIButtonComponent>();
}

geComponentRef UIButtonComponent::clone() const
{
    geComponentRef cloned = instantiate();
    cloned->setData(getData()->template clone<UIButtonComponentData>());
    return cloned;
}

void UIButtonComponent::createBackgroundGraphic()
{
    if(backgroundGraphic_)
        return;

    std::vector<Vec2D> path = {
        Vec2D(.0f, .0f),
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    auto graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
    backgroundGraphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    backgroundGraphic_->setModelTransform(calculateVirtualScreenPos(), 0.f, calculateVirtualScreenSize());
    graphicsEngine()->registerGraphic(backgroundGraphic_);
    backgroundGraphic_->setActive(getPropertyValue<bool>("visible"));

    // move the text position to center inside the button
    Vec2D textMargins = calculateVirtualScreenSize() - getTextSize();
    if(textMargins.x > 0 )
        textMargins.x /=2;
    else
        textMargins.x = 0;

    if(textMargins.y > 0 )
        textMargins.y /=2;
    else
        textMargins.y = 0;

    setTextModelTransform(calculateVirtualScreenPos() + textMargins, 0.f, Vec2D(1, 1));
}

void UIButtonComponent::onUnclick()
{
    if(command_)
        command_->execute();
    else
        std::cerr << "warning: " << gameObject()->name() << " doesn't have a command" << std::endl;
}

void UIButtonComponent::onHoverIn()
{
    changeColor(true);
}

void UIButtonComponent::onHoverOut()
{
    changeColor(false);
}

void UIButtonComponent::onVisibleChanged()
{
    UITextComponent::onVisibleChanged();
    if(backgroundGraphic_)
        backgroundGraphic_->setActive(getPropertyValue<bool>("visible"));
}

ComponentDataRef UIButtonComponent::instantiateData() const
{
    return std::make_shared<UIButtonComponentData>();
}
}
