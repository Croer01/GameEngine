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
    createBackgroundGraphic();
    backgroundGraphic_->setTintColor(background_);
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
    backgroundGraphic_->setActive(visible());
}

void UIPanelComponent::background(const geColor &color)
{
    background_ = color;
    if(backgroundGraphic_)
        backgroundGraphic_->setTintColor(background_);
}

geColor UIPanelComponent::background() const
{
    return background_;
}

void UIPanelComponent::onVisibleChanged()
{
    if(backgroundGraphic_)
        backgroundGraphic_->setActive(visible());
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
    auto compCloned = std::dynamic_pointer_cast<UIPanelComponent>(cloned);
    auto other = shared_from_this();
    auto thisRef = std::dynamic_pointer_cast<const UIPanelComponent>(other);
    compCloned->copyProperties<UIPanelComponent>(thisRef);

    return cloned;
}

PropertySetBase *UIPanelComponent::getProperties() const
{
    PropertySetBase *base = UIControlComponent::getProperties();
    auto *properties = new PropertySet<UIPanelComponent>(base);

    properties->add(new Property<UIPanelComponent, geColor>(
            "background",
            &UIPanelComponent::background,
            &UIPanelComponent::background,
            geColor(1.f)
    ));

    return properties;
}
}
