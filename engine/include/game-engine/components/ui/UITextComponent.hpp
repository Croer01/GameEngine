//
// Created by adria on 11/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP


#include "UIControlComponent.hpp"
#include "../../../../private/graphics/font/Text.hpp"

namespace GameEngine {
class PUBLICAPI UITextComponent : public UIControlComponent
{
    std::shared_ptr<Internal::Text> graphicText_;
    std::string text_;
    std::string font_;
    int fontSize_;

    void UpdateTextGraphic();
protected:
    geComponentRef instantiate() const override;
    void setTextModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale);
    std::shared_ptr<Internal::Text> getGraphicText() const;
public:
    virtual ~UITextComponent();

    PropertySetBase *getProperties() const override;

    void init() override;

    int fontSize() const;
    void fontSize(const int &size);

    std::string font() const;
    void font(const std::string &fontName);

    void text(const std::string &value);
    std::string text() const;

    Vec2D getTextSize() const;
};
}


#endif //GAMEENGINEEDITOR_UITEXTCOMPONENT_HPP