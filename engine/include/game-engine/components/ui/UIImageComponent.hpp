//
// Created by adria on 14/03/2020.
//

#ifndef GAMEDEVWARS_UIIMAGECOMPONENT_HPP
#define GAMEDEVWARS_UIIMAGECOMPONENT_HPP

#include <game-engine/components/ui/UIControlComponent.hpp>
#include "../../../../private/graphics/GraphicHolder.hpp"

namespace GameEngine
{
class UIImageComponent : public UIControlComponent
{
    std::shared_ptr<Internal::Graphic> graphicLoaded_;
    std::shared_ptr<Internal::GraphicHolder> graphic_;
    geColor color_;
    bool visible_;
    std::string filePath_;
    std::string anchor_;

    void updateGraphicRef();

public:
    virtual ~UIImageComponent();

protected:
    geComponentRef instantiate() const override;

    virtual void onVisibleChanged();
public:
    PropertySetBase *getProperties() const override;

protected:
    geComponentRef clone() const override;

public:
    void init() override;

    void filepath(const std::string &path);
    std::string filepath() const;

    void anchor(const std::string &anchor);
    std::string anchor() const;

    void color(const geColor &value);
    geColor color() const;
};
}

#endif //GAMEDEVWARS_UIIMAGECOMPONENT_HPP
