//
// Created by adria on 12/01/2019.
//

#include "GameManagerComponent.hpp"
#include "src/SceneManager.hpp"

std::shared_ptr<Component> GameManagerComponent::Clone() {
    auto clone = std::make_shared<GameManagerComponent>();
    clone->lives_ = lives_;
    return clone;
}

void GameManagerComponent::init() {
    currentLives_ = lives_;
    player_ = SceneManager::GetInstance().findObjectByName("Player")->getComponent<PlayerComponent>();
    if(auto player = player_.lock())
        player->registerObserver(this);
    enemyManager_ = SceneManager::GetInstance().findObjectByName("EnemyManager")->getComponent<EnemyManagerComponent>();
    lifesCounter_ = SceneManager::GetInstance()
            .findObjectByName("LifesCounter")->
            findChildByName("Label")->
            getComponent<TextComponent>();

    if(auto lifesCounter = lifesCounter_.lock())
        lifesCounter->setText("X" + std::to_string(currentLives_));
}

void GameManagerComponent::lostLive() {
    currentLives_--;
    if(currentLives_ < 0) {
        parent_->setActive(false);
        SceneManager::GetInstance().changeScene("StartMenu");
    } else {
        if(auto lifesCounter = lifesCounter_.lock())
            lifesCounter->setText("X" + std::to_string(currentLives_));
    }
}

void GameManagerComponent::fromFile(const YAML::Node &componentConfig) {
    lives_ = componentConfig["lives"].as<int>(0);
}

void GameManagerComponent::onEvent(const Subject<PlayerEvents> &target, const PlayerEvents &event, void *args) {
    if(event == PlayerEvents::KILLED)
        lostLive();
}
