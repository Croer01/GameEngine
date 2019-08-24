//
// Created by adria on 23/08/2019.
//

#ifndef GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
#define GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/Command.hpp>
#include "../../../private/graphics/Graphic.hpp"
#include "../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
class UIButtonComponent : public geComponentInstantiable<UIButtonComponent>
{
    bool pressed_;
    bool hover_;
    std::vector<Vec2D> path_;
    std::shared_ptr<Internal::Graphic> graphicLoaded_;
    std::shared_ptr<Internal::GraphicHolder> graphic_;
    CommandRef command_;
    void changeColor(bool isHover);
public:
    UIButtonComponent();
    virtual ~UIButtonComponent();
    void init() override;
    void Update(float elapsedTime) override;
    void setCommand(const CommandRef &command);
};
}


#endif //GAMEENGINEEDITOR_UIBUTTONCOMPONENT_HPP
