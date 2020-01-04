//
// Created by adria on 11/09/2019.
//

#include <game-engine/InputManager.hpp>
#include "game-engine/components/ui/UIControlComponent.hpp"

namespace GameEngine {

void UIControlComponent::screenPos(const Vec2D &pos)
{
    screenPos_ = pos;
}

Vec2D UIControlComponent::screenPos() const
{
    return screenPos_;
}

void UIControlComponent::screenSize(const Vec2D &size)
{
    screenSize_ = size;
}

Vec2D UIControlComponent::screenSize() const
{
    return screenSize_;
}

void UIControlComponent::Update(float elapsedTime)
{
    const Vec2D &mousePos = InputManager::GetInstance().getMousePosition();
    Vec2D min = screenPos();
    Vec2D max = min + screenSize();
    bool prevHover = hover_;
    hover_ = min.x <= mousePos.x && mousePos.x <= max.x && min.y <= mousePos.y && mousePos.y <= max.y;

    if(prevHover != hover_){
        if(hover_)
            onHoverIn();
        else
            onHoverOut();
    }

    if(hover_ && InputManager::GetInstance().isMouseButtonDown(MouseButton::LEFT))
    {
        onClick();
        if(!focused_)
        {
            focused_ = true;
            onFocusChanged();
        }
    }
    //TODO: change this to a Centralized Focus Manager
    else if(!hover_ && InputManager::GetInstance().isMouseButtonDown(MouseButton::LEFT))
    {
        if(focused_)
        {
            focused_ = false;
            onFocusChanged();
        }
    }
}

bool UIControlComponent::isFocused() const
{
    return focused_;
}

PropertySetBase *UIControlComponent::getProperties() const
{
    auto *properties = new PropertySet<UIControlComponent>();

    properties->add(new Property<UIControlComponent, Vec2D>(
            "screenPos",
            &UIControlComponent::screenPos,
            &UIControlComponent::screenPos,
            Vec2D(),
            true
    ));

    properties->add(new Property<UIControlComponent, Vec2D>(
            "screenSize",
            &UIControlComponent::screenSize,
            &UIControlComponent::screenSize,
            Vec2D(),
            true
    ));
    return properties;

}
}
