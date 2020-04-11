//
// Created by adria on 11/09/2019.
//

#include <game-engine/InputManager.hpp>
#include "game-engine/components/ui/UIControlComponent.hpp"
#include "../../../private/Game.hpp"

namespace GameEngine {

void UIControlComponent::preInit()
{
    graphicsEngine_ = dynamic_cast<Internal::Game*>(gameObject()->game())->graphicsEngine();
}

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
    if(!visible_)
        return;

    InputManager *inputManager = gameObject()->game()->input();

    const Vec2D &mousePos = inputManager->getMousePosition();
    Vec2D min = calculateVirtualScreenPos();
    Vec2D max = min + calculateVirtualScreenSize();
    bool prevHover = hover_;
    hover_ = min.x <= mousePos.x && mousePos.x <= max.x && min.y <= mousePos.y && mousePos.y <= max.y;

    if(prevHover != hover_){
        if(hover_)
            onHoverIn();
        else
            onHoverOut();
    }

    if(hover_ && inputManager->isMouseButtonDown(MouseButton::LEFT))
    {
        onClick();
        if(!focused_)
        {
            focused_ = true;
            onFocusChanged();
        }
    }
    //TODO: change this to a Centralized Focus Manager
    else if(!hover_ && inputManager->isMouseButtonDown(MouseButton::LEFT))
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

    properties->add(new Property<UIControlComponent, std::string>(
            "id",
            &UIControlComponent::id,
            &UIControlComponent::id,
            "",
            true
    ));

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
    properties->add(new Property<UIControlComponent, bool>(
            "visible",
            &UIControlComponent::visible,
            &UIControlComponent::visible,
            true,
            false
    ));
    return properties;
}

void UIControlComponent::id(const std::string &value)
{
    id_ = value;
}

std::string UIControlComponent::id() const
{
    return id_;
}

bool UIControlComponent::visible() const
{
    return visible_;
}

void UIControlComponent::visible(const bool &value)
{
    visible_ = value;
    onVisibleChanged();
}

Vec2D UIControlComponent::calculateVirtualScreenPos() const
{
    geScreen *screen = gameObject()->game()->screen();
    Vec2D position = screenPos();
    position.x *= static_cast<float>(screen->virtualWidth());
    position.y *= static_cast<float>(screen->virtualHeight());
    return position;
}

Vec2D UIControlComponent::calculateVirtualScreenSize() const
{
    geScreen *screen = gameObject()->game()->screen();
    Vec2D size = screenSize();
    size.x *= static_cast<float>(screen->virtualWidth());
    size.y *= static_cast<float>(screen->virtualHeight());
    return size;
}

Internal::GraphicsEngine *UIControlComponent::graphicsEngine()
{
    return graphicsEngine_;
}

}
