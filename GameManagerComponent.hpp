//
// Created by adria on 12/01/2019.
//

#ifndef SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP
#define SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP

#include "src/Component.hpp"
#include "PlayerComponent.hpp"
#include "EnemyManagerComponent.hpp"

COMPONENT(GameManagerComponent)
class GameManagerComponent : public Component, public Observer<PlayerEvents> {
    std::weak_ptr<PlayerComponent> player_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    std::weak_ptr<TextComponent> lifesCounter_;

    int lives_;
    int currentLives_;

    float playerKilledTimeAcumulator_;
    float playerKilledTime_;
    bool playerKilled_;

    void lostLive();
public:
    void init() override;

    void Update(float elapsedTime) override;

    void fromFile(const YAML::Node &componentConfig) override;

    std::shared_ptr<Component> Clone() override;

    void onEvent(const Subject<PlayerEvents> &target, const PlayerEvents &event, void *args) override;
};


#endif //SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP
