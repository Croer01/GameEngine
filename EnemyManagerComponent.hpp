//
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
#define SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP


#include "src/Component.hpp"
#include <glm/glm.hpp>
#include <random>

class MotherShipComponent;

enum class EnemyManagerEvents{
    ScoreChanged
};

COMPONENT(EnemyManagerComponent)
class EnemyManagerComponent : public Component, public Subject<EnemyManagerEvents>,
        public std::enable_shared_from_this<EnemyManagerComponent> {

    struct RowsConfig{
        int enemiesPerRow;
        std::vector<std::string> enemiesType;
        int horizontalMargin;
        int verticalMargin;
    };

    RowsConfig rowsConfig_;
    std::vector<std::shared_ptr<GameObject>> enemies_;
    glm::vec4 boundingBox_;
    std::vector<std::shared_ptr<GameObject>> bullets_;
    std::weak_ptr<MotherShipComponent> mothership_;
    int bulletsNum_;
    float bulletFrequency_;
    float currentBulletFrequency_;
    float bulletTimeAcumulator_;
    float mothershipFrequency_;
    float mothershipTimeAcumulator_;
    std::mt19937 rng_;

    glm::vec3 currentPosition_;
    float speed_;
    float currentSpeed_;
    float scaleFactor_;
    int score_;

    void updateBoundingBox();

    void checkMoveToNextLevel();
public:
    virtual ~EnemyManagerComponent();

    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void enemyKilled(int EnemyKilledpoints);
    int getScore() const;

};


#endif //SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
