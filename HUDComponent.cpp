//
// Created by adria on 04/11/2018.
//

#include "HUDComponent.hpp"
#include "src/InputManager.hpp"
#include "src/SceneManager.hpp"

std::shared_ptr<Component> HUDComponent::Clone() {
    return std::make_shared<HUDComponent>();
}

void HUDComponent::init() {
    playerScore_ = parent_->getComponent<TextComponent>();
    std::shared_ptr<GameObject> enemyManagerObject = SceneManager::GetInstance().findObjectByName("EnemyManager");
    enemyManager_ = enemyManagerObject->getComponent<EnemyManagerComponent>();
    enemyManager_->registerObserver(this);

    score_ = 0;
}

void HUDComponent::onEvent(const Subject<EnemyManagerEvents> &target, const EnemyManagerEvents &event, void *args) {
    const EnemyManagerComponent &enemyManager = (const EnemyManagerComponent &)target;
    playerScore_->setText("Score: " + std::to_string(enemyManager.getScore()));
}

HUDComponent::~HUDComponent() {
    enemyManager_->unregisterObserver(this);
}