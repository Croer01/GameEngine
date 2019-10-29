//
// Created by adria on 23/08/2019.
//

#include <game-engine/InputManager.hpp>
#include <iostream>
#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/GeometryComponent.hpp>
#include "game-engine/components/ui/UIButtonComponent.hpp"
#include "../../../private/graphics/font/FontManager.hpp"
#include "../../../private/graphics/GraphicsEngine.hpp"

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
    backgroundGraphic_->setTintColor(isHover ? hoverBackground_ : background_);
}

void UIButtonComponent::setCommand(const CommandRef &command)
{
    command_ = command;
}

UIButtonComponent::~UIButtonComponent()
{
    Internal::GraphicsEngine::GetInstance().unregisterGraphic(backgroundGraphic_);
}

geComponentRef UIButtonComponent::instantiate() const
{
    return std::make_shared<UIButtonComponent>();
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
    backgroundGraphic_->setModelTransform(screenPos(), Vec2D(0.f, 0.f), screenSize());
    Internal::GraphicsEngine::GetInstance().registerGraphic(backgroundGraphic_);

    // move the text position to center inside the button
    Vec2D textMargins = screenSize() - getTextSize();
    if(textMargins.x > 0 )
        textMargins.x /=2;
    else
        textMargins.x = 0;

    if(textMargins.y > 0 )
        textMargins.y /=2;
    else
        textMargins.y = 0;

    setTextModelTransform(screenPos() + textMargins, Vec2D(), Vec2D(1, 1));
}

void UIButtonComponent::background(const geColor &color)
{
    background_ = color;
}

geColor UIButtonComponent::background() const
{
    return background_;
}

void UIButtonComponent::hoverBackground(const geColor &color)
{
    hoverBackground_ = color;
}

geColor UIButtonComponent::hoverBackground() const
{
    return hoverBackground_;
}

void UIButtonComponent::onClick()
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
}
