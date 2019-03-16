//
// Created by adria on 12/01/2019.
//

#include <game-engine/InputManager.hpp>
#include "GameManagerComponent.hpp"

void GameManagerComponent::init() {
    currentLives_ = lives_;
    player_ = gameObject()->game().findObjectByNameInCurrentScene("Player")->getComponent<PlayerComponent>();
    if(auto player = player_.lock())
        player->registerObserver(this);
    enemyManager_ = gameObject()->game().findObjectByNameInCurrentScene("EnemyManager")->getComponent<EnemyManagerComponent>();
    lifesCounter_ = gameObject()->game().
            findObjectByNameInCurrentScene("LifesCounter")->
            findChildByName("Label")->
            getComponent<GameEngine::TextComponent>();

    if(auto lifesCounter = lifesCounter_.lock())
        lifesCounter->text("X" + std::to_string(currentLives_));

    playerExplosionSound_ = gameObject()->getComponent<GameEngine::AudioComponent>();

    playerKilledTimeAcumulator_ = 0.f;
    playerKilledTime_ = 2.f;
    playerKilled_ = false;
}

GameEngine::PropertySetBase *GameManagerComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<GameManagerComponent>(this);

    properties->add(new GameEngine::Property<GameManagerComponent, int>(
            "lives",
            this,
            &GameManagerComponent::lives,
            &GameManagerComponent::lives,
            0));

    return properties;
}

void GameManagerComponent::lostLive() {
    currentLives_--;
    if(currentLives_ >= 0)
        if(auto lifesCounter = lifesCounter_.lock())
            lifesCounter->text("X" + std::to_string(currentLives_));
}

void GameManagerComponent::onEvent(const GameEngine::Subject<PlayerEvents> &target, const PlayerEvents &event, void *args) {
    if(event == PlayerEvents::KILLED) {
        lostLive();
        playerKilled_ = true;
        playerKilledTimeAcumulator_ = 0;
        enemyManager_.lock()->setPause(true);
        playerExplosionSound_.lock()->play();
    }
}

void GameManagerComponent::Update(float elapsedTime) {
    if(playerKilled_){
        if(playerKilledTime_ <= playerKilledTimeAcumulator_) {
            if(currentLives_ < 0) {
                gameObject()->game().changeScene("StartMenu");
            } else {
                player_.lock()->restore();
                playerKilled_ = false;
                enemyManager_.lock()->setPause(false);
            }
        }
        playerKilledTimeAcumulator_ += elapsedTime;
    }

    if(GameEngine::InputManager::GetInstance().isKeyDown(GameEngine::KeyCode::KEY_m)) {
        gameObject()->game().audio().muteAll(!gameObject()->game().audio().muteAll());
    }
}

void GameManagerComponent::lives(const int &numLives) {
    lives_ = numLives;
}

int GameManagerComponent::lives() const {
    return lives_;
}
