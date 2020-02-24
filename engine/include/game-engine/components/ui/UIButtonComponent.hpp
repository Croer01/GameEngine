//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP

#include <game-engine/Command.hpp>
#include "UITextComponent.hpp"
#include "../../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
class PUBLICAPI UIButtonComponent : public UITextComponent
{
    CommandRef command_;
    std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;
    geColor background_;
    geColor hoverBackground_;

    void changeColor(bool isHover);
protected:
    geComponentRef instantiate() const override;
    virtual geComponentRef clone() const;
    void createBackgroundGraphic();
    void onClick() override;
    void onHoverIn() override;
    void onHoverOut() override;
public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
    PropertySetBase *getProperties() const override;

    void init() override;
    void setCommand(const CommandRef &command);

    void background(const geColor &color);
    geColor background() const;

    void hoverBackground(const geColor &color);
    geColor hoverBackground() const;
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
