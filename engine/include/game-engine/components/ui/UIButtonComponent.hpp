//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/Command.hpp>
#include <game-engine/components/ui/UITextComponent.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine {

class PUBLICAPI UIButtonComponentData : public UITextComponentData
{
public:
    UIButtonComponentData():
        UITextComponentData()
    {
        createProperty<geColor>("background", geColor(1.f));
        createProperty<geColor>("hoverBackground", geColor(.8f));
    }
};

class PUBLICAPI UIButtonComponent : public UITextComponent
{
    CommandRef command_;
    std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;

    void changeColor(bool isHover);
protected:
    geComponentRef instantiate() const override;
protected:
    virtual geComponentRef clone() const;
    void createBackgroundGraphic();
    void onClick() override;
    void onHoverIn() override;
    void onHoverOut() override;

    void onVisibleChanged() override;

public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
    ComponentDataRef instantiateData() const override;

    void init() override;
    void setCommand(const CommandRef &command);
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
