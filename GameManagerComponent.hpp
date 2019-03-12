//
// Created by adria on 12/01/2019.
//

#ifndef SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP
#define SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP

#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/AudioComponent.hpp>
#include "PlayerComponent.hpp"
#include "EnemyManagerComponent.hpp"

COMPONENT(GameManagerComponent)
class GameManagerComponent : public GameEngine::geComponentInstantiable<GameManagerComponent>, public GameEngine::Observer<PlayerEvents> {
    std::weak_ptr<PlayerComponent> player_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    std::weak_ptr<GameEngine::TextComponent> lifesCounter_;
    std::weak_ptr<GameEngine::AudioComponent> playerExplosionSound_;

    int lives_;
    int currentLives_;

    float playerKilledTimeAcumulator_;
    float playerKilledTime_;
    bool playerKilled_;

    void lostLive();
public:
    void init() override;

    void Update(float elapsedTime) override;

    void onEvent(const GameEngine::Subject<PlayerEvents> &target, const PlayerEvents &event, void *args) override;

protected:
    GameEngine::PropertySetBase *configureProperties() override;

public:

    void lives(const int &numLives);
    int lives() const;
};


#endif //SPACEINVADERS_GAMEMANAGERCOMPONENT_HPP
