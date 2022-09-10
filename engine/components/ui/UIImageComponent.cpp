//
// Created by adria on 14/03/2020.
//

#include <game-engine/components/ui/UIImageComponent.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
void UIImageComponent::init() {
    setFilePathPropertyObserver("filePath", [this](){ updateGraphicRef(); });
    setPropertyObserver<geColor>("tint", [this](){ updateTint(); });
    setEnumPropertyObserver("anchor", [this](){ updateAnchor(); });

    updateGraphicRef();
}

UIImageComponent::~UIImageComponent() {
    if(graphic_)
        graphicsEngine()->unregisterGraphic(graphic_);
}

void UIImageComponent::updateGraphicRef() {
    if(graphicsEngine() == nullptr)
        return;

    if(graphic_)
        graphicsEngine()->unregisterGraphic(graphic_);

    std::string filePath = getFilePathPropertyValue("filePath");
    if(filePath.empty()){
        graphicLoaded_.reset();
        graphic_.reset();
    } else {
        graphicLoaded_ = std::make_shared<Internal::GraphicSprite>(filePath);
        graphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
        graphicsEngine()->unregisterGraphic(graphic_);
        updateTint();
        updateAnchor();
        Vec2D calculatedUIScale = calculateVirtualScreenSize();
        Vec2D scale(calculatedUIScale.x / graphicLoaded_->getWidth(), calculatedUIScale.y / graphicLoaded_->getHeight());
        graphic_->setModelTransform(calculateVirtualScreenPos(), 0.f, scale);
        onVisibleChanged();
    }
}

void UIImageComponent::updateAnchor() {
    if(graphic_)
    {
        std::string anchor = getEnumPropertyValue("anchor");
        GameEngine::Internal::GraphicAnchor graphicAnchor = Internal::parseStringToGraphicAnchor(anchor);
        graphic_->setAnchor(graphicAnchor);
    }
}

void UIImageComponent::updateTint()
{
    if(graphic_){
        geColor tint = getPropertyValue<geColor>("tint");
        graphic_->setTintColor(tint);
    }
}

geComponentRef UIImageComponent::instantiate() const
{
    return std::make_shared<UIImageComponent>();
}

void UIImageComponent::onVisibleChanged()
{
    UIControlComponent::onVisibleChanged();
    if(graphic_)
    {
        bool visible = getPropertyValue<bool>("visible");
        graphic_->setActive(visible);
    }
}

geComponentRef UIImageComponent::clone() const
{
    geComponentRef cloned = instantiate();
    cloned->setData(getData()->template clone<UIImageComponentData>());
    return cloned;
}

ComponentDataRef UIImageComponent::instantiateData() const
{
    return std::make_shared<UIImageComponentData>();
}

}
