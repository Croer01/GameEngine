//
// Created by adria on 04/11/2018.
//

#include "HUDComponent.hpp"

void HUDComponent::init() {
    playerScore_ = gameObject()->getComponent<GameEngine::TextComponent>();
    GameEngine::geGameObjectRef enemyManagerObject = gameObject()->game().findObjectByNameInCurrentScene("EnemyManager");
    enemyManager_ = enemyManagerObject->getComponent<EnemyManagerComponent>();
    if(auto enemyManager = enemyManager_.lock())
        enemyManager->registerObserver(this);

    score_ = 0;
}

void HUDComponent::onEvent(const GameEngine::Subject<EnemyManagerEvents> &target, const EnemyManagerEvents &event, void *args) {
    const EnemyManagerComponent &enemyManager = (const EnemyManagerComponent &)target;
    if(auto playerScore = playerScore_.lock())
        playerScore->text("Score: " + std::to_string(enemyManager.getScore()));
}

HUDComponent::~HUDComponent() {
    if(auto enemyManager = enemyManager_.lock())
        enemyManager->unregisterObserver(this);
}
