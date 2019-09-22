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
    Vec2D min = screenPos();
    Vec2D max = min + screenSize();
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
    UITextComponent::init();
    createGeomGraphic();
}

void UIButtonComponent::changeColor(bool isHover)
{
    createGeomGraphic();
    graphicGeom_->setTintColor(isHover ? hoverBackground_ : background_);
}

void UIButtonComponent::setCommand(const CommandRef &command)
{
    command_ = command;
}

UIButtonComponent::~UIButtonComponent()
{
    Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphicGeom_);
}

geComponentRef UIButtonComponent::instantiate() const
{
    return std::make_shared<UIButtonComponent>();
}

PropertySetBase *UIButtonComponent::configureProperties()
{
    auto *base = UITextComponent::configureProperties();
    auto *properties = new PropertySet<UIButtonComponent>(this, base);

    properties->add(new Property<UIButtonComponent, geColor>(
        "background",
        this,
        &UIButtonComponent::background,
        &UIButtonComponent::background,
        geColor(1.f)
    ));

    properties->add(new Property<UIButtonComponent, geColor>(
        "hoverBackground",
        this,
        &UIButtonComponent::hoverBackground,
        &UIButtonComponent::hoverBackground,
        geColor(.8f)
    ));

    return properties;
}

void UIButtonComponent::createGeomGraphic()
{
    if(graphicGeom_)
        return;

    std::vector<Vec2D> path = {
        Vec2D(.0f, .0f),
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    auto graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
    graphicGeom_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    graphicGeom_->setModelTransform(screenPos(), Vec2D(0.f, 0.f), screenSize());
    Internal::GraphicsEngine::GetInstance().registerGraphic(graphicGeom_);
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
}
