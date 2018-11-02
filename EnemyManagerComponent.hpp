//
// Created by adria on 29/10/2018.
//

#ifndef SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
#define SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP


#include "src/Component.hpp"
#include <glm/glm.hpp>

COMPONENT(EnemyManagerComponent)
class EnemyManagerComponent : public Component{

    struct RowsConfig{
        int enemiesPerRow;
        std::vector<std::string> enemiesType;
        int horizontalMargin;
        int verticalMargin;
    };

    RowsConfig rowsConfig_;
    std::vector<std::shared_ptr<GameObject>> enemies_;
    glm::vec4 boundingBox_;

    float speed_;
    float currentSpeed_;
    float scaleFactor_;

    void updateBoundingBox();

    void checkMoveToNextLevel();
public:
    ~EnemyManagerComponent() override;

    void init() override;

    void Update(float elapsedTime) override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void enemyKilled();

};


#endif //SPACEINVADERS_ENEMYMANAGERCOMPONENT_HPP
