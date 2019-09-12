//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP

#include <game-engine/Command.hpp>
#include "UITextComponent.hpp"

namespace GameEngine {
class UIButtonComponent : public UITextComponent
{
    bool pressed_;
    bool hover_;
    std::vector<Vec2D> path_;
    CommandRef command_;
    std::string text_;
    void changeColor(bool isHover);
    std::shared_ptr<Internal::Text> graphicText_;
    std::shared_ptr<Internal::GraphicHolder> graphicGeom_;
public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
protected:
    PropertySetBase *configureProperties() override;
public:
    void init() override;
    void Update(float elapsedTime) override;
    void setCommand(const CommandRef &command);

    std::string text() const;
    void text(const std::string &value);
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
