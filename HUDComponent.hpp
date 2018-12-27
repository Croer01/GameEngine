//
// Created by adria on 04/11/2018.
//

#ifndef SPACEINVADERS_HUDCOMPONENT_HPP
#define SPACEINVADERS_HUDCOMPONENT_HPP


#include "src/Component.hpp"
#include "src/components/TextComponent.hpp"
#include "EnemyManagerComponent.hpp"

COMPONENT(HUDComponent)
class HUDComponent : public Component, public Observer<EnemyManagerEvents>{
    std::weak_ptr<TextComponent> playerScore_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    int score_;
public:
    virtual ~HUDComponent();

    void init() override;

    std::shared_ptr<Component> Clone() override;

    void onEvent(const Subject<EnemyManagerEvents> &target, const EnemyManagerEvents &event, void *args) override;
};


#endif //SPACEINVADERS_HUDCOMPONENT_HPP
