//
// Created by adria on 07/03/2020.
//


#include <game-engine/components/ui/UIPanelComponent.hpp>
#include <game-engine/internal/graphics/GraphicGeometry.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine
{

void UIPanelComponent::init()
{
    setPropertyObserver<geColor>("background",[this](){ onUpdateBackground(); });
    createBackgroundGraphic();
    onUpdateBackground();
}
void UIPanelComponent::createBackgroundGraphic()
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
    onVisibleChanged();
}

void UIPanelComponent::onUpdateBackground()
{
    if(backgroundGraphic_)
        backgroundGraphic_->setTintColor(getPropertyValue<geColor>("background"));
}

void UIPanelComponent::onVisibleChanged()
{
    UIControlComponent::onVisibleChanged();
    if(backgroundGraphic_)
    {
        backgroundGraphic_->setActive(getPropertyValue<bool>("visible"));
    }
}

UIPanelComponent::~UIPanelComponent()
{
    if(backgroundGraphic_)
        graphicsEngine()->unregisterGraphic(backgroundGraphic_);
}

geComponentRef UIPanelComponent::instantiate() const
{
    return std::make_shared<UIPanelComponent>();
}

geComponentRef UIPanelComponent::clone() const
{
    geComponentRef cloned = instantiate();
    cloned->setData(getData()->template clone<UIPanelComponentData>());
    return cloned;
}

ComponentDataRef UIPanelComponent::instantiateData() const
{
    return std::make_shared<UIPanelComponentData>();
}

}
