//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP

#include <game-engine/Command.hpp>
#include "UITextComponent.hpp"
#include "../../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
class UIButtonComponent : public UITextComponent
{
    bool pressed_;
    bool hover_;
    CommandRef command_;
    void changeColor(bool isHover);
    std::shared_ptr<Internal::GraphicHolder> graphicGeom_;
    geColor background_;
    geColor hoverBackground_;

protected:
    geComponentRef instantiate() const override;
    PropertySetBase *configureProperties() override;
    void createGeomGraphic();

public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
    void init() override;
    void Update(float elapsedTime) override;
    void setCommand(const CommandRef &command);

    void background(const geColor &color);
    geColor background() const;

    void hoverBackground(const geColor &color);
    geColor hoverBackground() const;
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
