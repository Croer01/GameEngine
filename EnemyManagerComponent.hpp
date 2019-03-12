//
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
#define SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/events/Subject.hpp>
#include <game-engine/Random.hpp>
#include <game-engine/geGameObject.hpp>

class MotherShipComponent;

enum class EnemyManagerEvents{
    ScoreChanged
};

COMPONENT(EnemyManagerComponent)
class EnemyManagerComponent : public GameEngine::geComponentInstantiable<EnemyManagerComponent>, public GameEngine::Subject<EnemyManagerEvents>,
        public std::enable_shared_from_this<EnemyManagerComponent> {

    struct RowsConfig{
        int enemiesPerRow;
        std::vector<std::string> enemiesType;
        GameEngine::Vec2D margins;
    };

    RowsConfig rowsConfig_;
    std::vector<GameEngine::geGameObjectRef> enemies_;
    GameEngine::Vec2D boundingBoxMin_;
    GameEngine::Vec2D boundingBoxMax_;
    std::vector<GameEngine::geGameObjectRef> bullets_;
    std::weak_ptr<MotherShipComponent> mothership_;
    int bulletsNum_;
    float bulletFrequency_;
    float currentBulletFrequency_;
    float bulletTimeAcumulator_;
    float mothershipFrequency_;
    float mothershipCurrentFrequency_;
    float mothershipVariation_;
    float mothershipTimeAcumulator_;
    GameEngine::Random rnd_;

    GameEngine::Vec2D currentPosition_;
    float speed_;
    float currentSpeed_;
    float scaleFactor_;
    int score_;

    bool paused_;

    void updateBoundingBox();

    void checkMoveToNextLevel();
public:
    virtual ~EnemyManagerComponent();

    virtual void init();

    void Update(float elapsedTime) override;
    virtual GameEngine::PropertySetBase *configureProperties();

    void enemyKilled(int EnemyKilledpoints);
    int getScore() const;
    void setPause(bool pause);

    void speed(const float &speedValue);
    float speed() const;

    void scaleFactor(const float &scaleValue);
    float scaleFactor() const;

    void bulletNum(const int &num);
    int bulletNum() const;

    void bulletFrequency(const float &frequency);
    float bulletFrequency() const;

    void mothershipFrequency(const float &frequency);
    float mothershipFrequency() const;

    void mothershipVariation(const float &variation);
    float mothershipVariation() const;

    void enemiesPerRow(const int &num);
    int enemiesPerRow() const;

    void margins(const GameEngine::Vec2D &margin);
    GameEngine::Vec2D margins() const;

    void rowsConfig(const std::vector<std::string> &config);
    std::vector<std::string> rowsConfig() const;
};


#endif //SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
