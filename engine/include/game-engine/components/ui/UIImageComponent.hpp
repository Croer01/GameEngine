//
// Created by adria on 14/03/2020.
//

#ifndef GAMEDEVWARS_UIIMAGECOMPONENT_HPP
#define GAMEDEVWARS_UIIMAGECOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/components/ui/UIControlComponent.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine
{

class PUBLICAPI UIImageComponentData : public UIControlComponentData
{
public:
    UIImageComponentData() :
        UIControlComponentData()
    {
        createPropertyFilePath("filePath", "", FileType::IMAGE, true);
        createProperty<geColor>("tint", geColor(1.f));
        createPropertyEnum("anchor", "TOP_LEFT", {
            "TOP_LEFT",
            "TOP_CENTER",
            "TOP_RIGHT",
            "MIDDLE_LEFT",
            "MIDDLE_CENTER",
            "MIDDLE_RIGHT",
            "BOTTOM_LEFT",
            "BOTTOM_CENTER",
            "BOTTOM_RIGHT"
        });
    }
};

class PUBLICAPI UIImageComponent : public UIControlComponent
{
    std::shared_ptr<Internal::Graphic> graphicLoaded_;
    std::shared_ptr<Internal::GraphicHolder> graphic_;

    void updateGraphicRef();
    void updateAnchor();
    void updateTint();

public:
    virtual ~UIImageComponent();

protected:
    geComponentRef instantiate() const override;

    virtual void onVisibleChanged();

protected:
    geComponentRef clone() const override;

public:
    void init() override;
    ComponentDataRef instantiateData() const override;
};
}

#endif //GAMEDEVWARS_UIIMAGECOMPONENT_HPP
