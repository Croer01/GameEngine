//
// Created by adria on 11/09/2019.
//

#include <game-engine/InputManager.hpp>
#include <game-engine/components/ui/UIControlComponent.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {

void UIControlComponent::preInit()
{
    graphicsEngine_ = gameObject()->game()->graphicsEngine();
    setPropertyObserver<bool>("visible",[this](){ onVisibleChanged(); });
}

void UIControlComponent::Update(float elapsedTime)
{
    if(!getPropertyValue<bool>("visible"))
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

    if(hover_)
    {
        if (inputManager->isMouseButtonDown(MouseButton::LEFT))
        {
            onClick();
            if(!focused_)
            {
                focused_ = true;
                onFocusChanged();
            }
        }
        else if (inputManager->isMouseButtonUp(MouseButton::LEFT))
        {
            onUnclick();
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

Vec2D UIControlComponent::calculateVirtualScreenPos() const
{
    geScreen *screen = gameObject()->game()->screen();
    Vec2D position = getPropertyValue<Vec2D>("screenPos");
    position.x *= static_cast<float>(screen->virtualWidth());
    position.y *= static_cast<float>(screen->virtualHeight());
    return position;
}

Vec2D UIControlComponent::calculateVirtualScreenSize() const
{
    geScreen *screen = gameObject()->game()->screen();
    Vec2D size = getPropertyValue<Vec2D>("screenSize");
    size.x *= static_cast<float>(screen->virtualWidth());
    size.y *= static_cast<float>(screen->virtualHeight());
    return size;
}

Internal::GraphicsEngine *UIControlComponent::graphicsEngine()
{
    return graphicsEngine_;
}

}
