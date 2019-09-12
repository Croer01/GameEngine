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
    : pressed_(false)
{}

void UIButtonComponent::Update(float elapsedTime)
{
    const Vec2D &mousePos = InputManager::GetInstance().getMousePosition();
    Vec2D min = gameObject()->position();
    Vec2D max = min + gameObject()->scale();
    bool prevHover = hover_;
    hover_ = min.x <= mousePos.x && mousePos.x <= max.x && min.y <= mousePos.y && mousePos.y <= max.y;

    if(prevHover != hover_)
        changeColor(hover_);

    if(hover_ && InputManager::GetInstance().isMouseButtonDown(MouseButton::LEFT))
    {
        if(command_)
            command_->execute();
        else
            std::cerr << "warning: " << gameObject()->name() << " doesn't have a command" << std::endl;
    }
}

void UIButtonComponent::init()
{
    path_ = {
        Vec2D(.0f, .0f),
        Vec2D(10.f, .0f),
        Vec2D(10.f, 10.f),
        Vec2D(.0f, 10.f)
    };

    auto geomComponent = std::make_shared<GeometryComponent>();
    geomComponent->path(path_);
    gameObject()->addComponent(geomComponent);

    auto textComponent = std::make_shared<TextComponent>();
    textComponent->font("assets/arial.ttf");
    textComponent->fontSize(std::max(gameObject()->scale().y -2,1.f));
    textComponent->fontSize(20.f);
    textComponent->text(text());
    gameObject()->addComponent(textComponent);
}

void UIButtonComponent::changeColor(bool isHover)
{
    if(graphicGeom_)
        graphicGeom_->setTintColor(geColor(isHover ? .8f : 1.f));
}

void UIButtonComponent::setCommand(const CommandRef &command)
{
    command_ = command;
}

UIButtonComponent::~UIButtonComponent()
{
    if (graphicGeom_)
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphicGeom_);

    if (graphicText_)
        Internal::GraphicsEngine::GetInstance().unregisterText(graphicText_);
}

PropertySetBase *UIButtonComponent::configureProperties()
{
    auto *properties = new PropertySet<UIButtonComponent>(this);

    properties->add(new Property<UIButtonComponent, std::string>(
        "text",
        this,
        &UIButtonComponent::text,
        &UIButtonComponent::text,
        ""));

    return properties;
}

std::string UIButtonComponent::text() const
{
    return text_;
}

void UIButtonComponent::text(const std::string &value)
{
    text_ = value;
}
}
