//
// Created by adria on 23/08/2019.
//

#include <game-engine/InputManager.hpp>
#include <iostream>
#include "game-engine/components/UIButtonComponent.hpp"
#include "../../private/graphics/GraphicGeometry.hpp"
#include "../../private/graphics/GraphicsEngine.hpp"

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
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path_);
    graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    graphic_->setModelTransform(gameObject()->position(), gameObject()->rotation(), gameObject()->scale());
    Internal::GraphicsEngine::GetInstance().registerGraphic(graphic_);
}

void UIButtonComponent::changeColor(bool isHover)
{
    graphic_->setTintColor(geColor(isHover ? .8f : 1.f));
}

void UIButtonComponent::setCommand(const CommandRef &command)
{
    command_ = command;
}

UIButtonComponent::~UIButtonComponent()
{
    if (graphic_)
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphic_);
}
}
