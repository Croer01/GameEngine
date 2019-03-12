//
// Created by adria on 04/11/2018.
//

#ifndef SPACEINVADERS_HUDCOMPONENT_HPP
#define SPACEINVADERS_HUDCOMPONENT_HPP


#include <game-engine/components/TextComponent.hpp>
#include "EnemyManagerComponent.hpp"

COMPONENT(HUDComponent)
class HUDComponent : public GameEngine::geComponentInstantiable<HUDComponent>, public GameEngine::Observer<EnemyManagerEvents>{
    std::weak_ptr<GameEngine::TextComponent> playerScore_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    int score_;
public:
    virtual ~HUDComponent();

    void init() override;

    void onEvent(const GameEngine::Subject<EnemyManagerEvents> &target, const EnemyManagerEvents &event, void *args) override;
};


#endif //SPACEINVADERS_HUDCOMPONENT_HPP
