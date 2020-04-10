//
// Created by adria on 14/03/2020.
//

#include "game-engine/components/ui/UIImageComponent.hpp"
#include "../private/graphics/GraphicsEngine.hpp"
#include "../../../private/graphics/GraphicSprite.hpp"
#include "../../../private/Game.hpp"

namespace GameEngine {
void UIImageComponent::init() {
    updateGraphicRef();
}

UIImageComponent::~UIImageComponent() {
    if(graphic_)
        std::dynamic_pointer_cast<Internal::Game>(gameObject()->game().lock())->graphicsEngine().unregisterGraphic(graphic_);
}

void UIImageComponent::filepath(const std::string &path) {
    filePath_ = path;
}

std::string UIImageComponent::filepath() const {
    return filePath_;
}

void UIImageComponent::updateGraphicRef() {
    if(!gameObject()->game().expired())
        return;

    Internal::GraphicsEngine &graphicsEngine = std::dynamic_pointer_cast<Internal::Game>(
        gameObject()->game().lock())->graphicsEngine();

    if(graphic_)
        graphicsEngine.unregisterGraphic(graphic_);

    if(filePath_.empty()){
        graphicLoaded_.reset();
        graphic_.reset();
    } else {
        graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath_);
        graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
        graphicsEngine.unregisterGraphic(graphic_);
        graphic_->setTintColor(color_);
        anchor(anchor_);
        Vec2D calculatedUIScale = calculateVirtualScreenSize();
        Vec2D scale(calculatedUIScale.x / graphicLoaded_->getWidth(), calculatedUIScale.y / graphicLoaded_->getHeight());
        graphic_->setModelTransform(calculateVirtualScreenPos(), 0.f, scale);
        graphic_->setActive(visible());
    }
}

std::string UIImageComponent::anchor() const {
    return anchor_;
}

void UIImageComponent::anchor(const std::string &anchor) {
    if(graphic_)
        graphic_->setAnchor(Internal::parseStringToGraphicAnchor(anchor));

    anchor_ = anchor;
}

void UIImageComponent::color(const geColor &value)
{
    color_ = value;
    if(graphic_)
        graphic_->setTintColor(color_);
}

geColor UIImageComponent::color() const
{
    return color_;
}
PropertySetBase *UIImageComponent::getProperties() const
{
    PropertySetBase *base = UIControlComponent::getProperties();
    auto *properties = new PropertySet<UIImageComponent>(base);

    properties->add(new PropertyFilePath<UIImageComponent>(
            "filePath",
            &UIImageComponent::filepath,
            &UIImageComponent::filepath,
            "",
            FileType::IMAGE,
            true));

    properties->add(new Property<UIImageComponent, geColor>(
            "tint",
            &UIImageComponent::color,
            &UIImageComponent::color,
            geColor(1.f)));

    properties->add(new PropertyEnum<UIImageComponent>(
            "anchor",
            &UIImageComponent::anchor,
            &UIImageComponent::anchor,
            "TOP_LEFT",
            {
                    "TOP_LEFT",
                    "TOP_CENTER",
                    "TOP_RIGHT",
                    "MIDDLE_LEFT",
                    "MIDDLE_CENTER",
                    "MIDDLE_RIGHT",
                    "BOTTOM_LEFT",
                    "BOTTOM_CENTER",
                    "BOTTOM_RIGHT"
            }));

    return properties;

}

geComponentRef UIImageComponent::instantiate() const
{
    return std::make_shared<UIImageComponent>();
}

void UIImageComponent::onVisibleChanged()
{
    if(graphic_)
        graphic_->setActive(visible());
}

geComponentRef UIImageComponent::clone() const
{
    geComponentRef cloned = instantiate();
    auto compCloned = std::dynamic_pointer_cast<UIImageComponent>(cloned);
    auto other = shared_from_this();
    auto thisRef = std::dynamic_pointer_cast<const UIImageComponent>(other);
    compCloned->copyProperties<UIImageComponent>(thisRef);

    return cloned;
}

}
