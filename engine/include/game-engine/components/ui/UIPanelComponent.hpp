//
// Created by adria on 07/03/2020.
//

#ifndef GAMEDEVWARS_UIPANELCOMPONENT_HPP
#define GAMEDEVWARS_UIPANELCOMPONENT_HPP

#include <game-engine/components/ui/UIControlComponent.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine {

class UIPanelComponentData : public UIControlComponentData
{
public:
    UIPanelComponentData() :
        UIControlComponentData()
    {
        createProperty<geColor>("background", geColor(1.f));
    }
};

class UIPanelComponent : public UIControlComponent
    {
        std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;

        void createBackgroundGraphic();
        void onUpdateBackground();
    protected:
        geComponentRef instantiate() const override;
        virtual geComponentRef clone() const;
        void onVisibleChanged() override;

    public:
        virtual ~UIPanelComponent();

        void init() override;
        ComponentDataRef instantiateData() const override;
    };

}

#endif //GAMEDEVWARS_UIPANELCOMPONENT_HPP
