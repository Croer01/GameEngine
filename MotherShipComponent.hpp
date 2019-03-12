//
// Created by adria on 23/11/2018.
//

#ifndef SPACEINVADERS_MOTHERSHIPCOMPONENT_HPP
#define SPACEINVADERS_MOTHERSHIPCOMPONENT_HPP

#include <game-engine/components/AudioComponent.hpp>
#include "EnemyManagerComponent.hpp"

class MotherShipComponent;
class MotherShipObserver : public GameEngine::Observer<GameEngine::GameObjectEvent>{
    std::weak_ptr<GameEngine::AudioComponent> sound_;
public:
    explicit MotherShipObserver(MotherShipComponent *mothership);
    void onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) override;
};

COMPONENT(MotherShipComponent);
class MotherShipComponent : public GameEngine::geComponentInstantiable<MotherShipComponent>, public GameEngine::Observer<GameEngine::GameObjectEvent> {
    int minPoints_;
    int maxPoints_;
    int points_;
    int pointsPerBullet_;

    float speed_;
    std::weak_ptr<GameEngine::geGameObject> playerBullet_;
    std::weak_ptr<EnemyManagerComponent> enemyManager_;
    std::weak_ptr<GameEngine::geGameObject> scoreText_;
    std::weak_ptr<GameEngine::geGameObject> explosion_;
    std::shared_ptr<MotherShipObserver> motherShipSoundObserver_;

public:
    void init() override;

    void Update(float elapsedTime) override;
    void kill();

    void onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event, void *args) override;

    //public Mothership functions
    void hide();

    void minPoints(const int &points);
    int minPoints() const;
    void maxPoints(const int &points);
    int maxPoints() const;
    void pointsPerBullet(const int &points);
    int pointsPerBullet() const;
    void speed(const float &speedValue);
    float speed() const;


protected:
    GameEngine::PropertySetBase *configureProperties() override;
};


#endif //SPACEINVADERS_MOTHERSHIPCOMPONENT_HPP
