//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP


#include <game-engine/api.hpp>
#include <game-engine/components/ui/UIControlComponent.hpp>
#include <game-engine/internal/graphics/font/Text.hpp>

namespace GameEngine {

class PUBLICAPI UITextComponentData : public UIControlComponentData
{
public:
    UITextComponentData() :
        UIControlComponentData()
    {
        createProperty<std::string>("text", "", true);
        createProperty<std::string>("font", "", true);
        createProperty<int>("fontSize", 0);
        createProperty<geColor>("foregroundColor", geColor(0.f));
    }
};

class PUBLICAPI PUBLICAPI UITextComponent : public UIControlComponent
{
    std::shared_ptr<Internal::Text> graphicText_;

    void UpdateTextGraphic();
protected:
    geComponentRef instantiate() const override;
    virtual geComponentRef clone() const;
    void setTextModelTransform(const Vec2D &position, float rotation, const Vec2D &scale);
    std::shared_ptr<Internal::Text> getGraphicText() const;

    virtual void onVisibleChanged();

public:
    virtual ~UITextComponent();
    ComponentDataRef instantiateData() const override;

    void init() override;

    // UI Text properties
    Vec2D getTextSize() const;
};
}


#endif //GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
