//
// Created by adria on 07/03/2020.
//

#ifndef GAMEDEVWARS_UIPANELCOMPONENT_HPP
#define GAMEDEVWARS_UIPANELCOMPONENT_HPP

#include <game-engine/components/ui/UIControlComponent.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine {
    class UIPanelComponent : public UIControlComponent
    {
        std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;
        geColor background_;

        void createBackgroundGraphic();
    protected:
        geComponentRef instantiate() const override;
        virtual geComponentRef clone() const;
        void onVisibleChanged() override;

    public:
        virtual ~UIPanelComponent();

        void init() override;
        PropertySetBase *getProperties() const override;

        void background(const geColor &color);
        geColor background() const;
    };

}

#endif //GAMEDEVWARS_UIPANELCOMPONENT_HPP
