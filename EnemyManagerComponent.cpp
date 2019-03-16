//
// Created by adria on 29/10/2018.
//

#include <iostream>
#include <ctime>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include "EnemyManagerComponent.hpp"
#include "EnemyComponent.hpp"
#include "MotherShipComponent.hpp"

void EnemyManagerComponent::init() {
    currentSpeed_ = speed_;
    currentPosition_ = gameObject()->position();
    currentBulletFrequency_ = bulletFrequency_;
    mothershipCurrentFrequency_ = rnd_.getRange(mothershipFrequency_ - mothershipVariation_,mothershipFrequency_ + mothershipVariation_);

    auto motherShip = gameObject()->game().findObjectByNameInCurrentScene("MotherShip");
    if(motherShip){
        mothership_ = motherShip->getComponent<MotherShipComponent>();
        motherShip->active(false);
    }

    //generate enemies
    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;
    const std::shared_ptr<EnemyManagerComponent> &manager = shared_from_this();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const GameEngine::geGameObjectRef &alien = gameObject()->game().createFromPrototype(rowsConfig_.enemiesType[i]);
            enemies_.push_back(alien);
            if (auto enemy = alien->getComponent<EnemyComponent>().lock())
                enemy->setEnemeyManager(manager);
        }
    }

    if(auto firstEnemy = enemies_[0]->getComponent<GameEngine::SpriteAnimatedComponent>().lock()){
        boundingBoxMin_ = GameEngine::Vec2D(0.f,0.f);
        boundingBoxMax_ = GameEngine::Vec2D(firstEnemy->getWidth() * columns,firstEnemy->getHeight() * rows);
    }

    //generate bullets
    for(int i = 0; i < bulletsNum_; i++){
        const GameEngine::geGameObjectRef &bullet = gameObject()->game().createFromPrototype("EnemyBullet");
        bullet->active(false);
        bullets_.push_back(bullet);
    }
    paused_ = false;
}

void EnemyManagerComponent::Update(float elapsedTime) {
    if(paused_)
        return;

    const GameEngine::geScreen &screen = gameObject()->game().screen();

    constexpr float min = 0;
    float max = screen.virtualWidth();
    float bottom = screen.virtualHeight();

    if(currentPosition_.y + boundingBoxMax_.y >= bottom)
        gameObject()->game().changeScene("StartMenu");

    if ((currentPosition_.x + boundingBoxMin_.x <= min && currentSpeed_ < 0) || (currentPosition_.x + boundingBoxMax_.x >= max && currentSpeed_ > 0)) {
        currentSpeed_ = -currentSpeed_;
        if(auto firstEnemy = enemies_[0]->getComponent<GameEngine::SpriteAnimatedComponent>().lock())
            currentPosition_ += GameEngine::Vec2D(0, firstEnemy->getHeight());
    }
    currentPosition_ += GameEngine::Vec2D(currentSpeed_ * elapsedTime, 0);

    int rows = rowsConfig_.enemiesType.size();
    int columns = rowsConfig_.enemiesPerRow;

    std::vector<int> enemiesActive;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            int index = i * columns + j;
            const GameEngine::geGameObjectRef &alien = enemies_[index];
            if(auto sprite = alien->getComponent<GameEngine::SpriteAnimatedComponent>().lock()) {
                alien->position(currentPosition_ + GameEngine::Vec2D(
                        j * sprite->getWidth() + j * rowsConfig_.margins.x,
                        i * sprite->getHeight() + i * rowsConfig_.margins.y));
            }
            if(alien->active())
                enemiesActive.push_back(index);
        }
    }

    bulletTimeAcumulator_ += elapsedTime;
    if(currentBulletFrequency_ <= bulletTimeAcumulator_) {
        int i = 0;
        GameEngine::geGameObjectRef bullet;
        while(!bullet && i < bullets_.size()){
            if(!bullets_[i]->active())
                bullet = bullets_[i];
            i++;
        }

        if(bullet) {
            int index = rnd_.getRange(0,enemiesActive.size() - 1);
            bullet->position(enemies_[enemiesActive[index]]->position());
            bullet->active(true);
        }
        bulletTimeAcumulator_ = 0;
    }

    if(auto mothership = mothership_.lock()) {
        if(!mothership->gameObject()->active())
            mothershipTimeAcumulator_ += elapsedTime;

        if(mothershipCurrentFrequency_ <= mothershipTimeAcumulator_) {
            if(!mothership->gameObject()->active())
                mothership->gameObject()->active(true);
            mothershipTimeAcumulator_ = 0;
            mothershipCurrentFrequency_ = rnd_.getRange(mothershipFrequency_ - mothershipVariation_,mothershipFrequency_ + mothershipVariation_);
            std::cout << "mothershipCurrentFrequency_: " << std::to_string(mothershipCurrentFrequency_)<< std::endl;
        }
    }
}

void EnemyManagerComponent::setPause(bool pause) {
    paused_ = pause;
    if(paused_){
        mothership_.lock()->hide();

        for (const GameEngine::geGameObjectRef &bullet : bullets_){
            bullet->active(false);
            bulletTimeAcumulator_ = 0;
        }
    }
}

GameEngine::PropertySetBase *EnemyManagerComponent::configureProperties() {
    auto *properties = new GameEngine::PropertySet<EnemyManagerComponent>(this);

    properties->add(new GameEngine::Property<EnemyManagerComponent, float>(
            "speed",
            this,
            &EnemyManagerComponent::speed,
            &EnemyManagerComponent::speed,
            0));
    properties->add(new GameEngine::Property<EnemyManagerComponent, float>(
            "scaleFactor",
            this,
            &EnemyManagerComponent::scaleFactor,
            &EnemyManagerComponent::scaleFactor,
            0));
    properties->add(new GameEngine::Property<EnemyManagerComponent, int>(
            "bulletsNum",
            this,
            &EnemyManagerComponent::bulletNum,
            &EnemyManagerComponent::bulletNum,
            0));
    properties->add(new GameEngine::Property<EnemyManagerComponent, float>(
            "bulletFrequency",
            this,
            &EnemyManagerComponent::bulletFrequency,
            &EnemyManagerComponent::bulletFrequency,
            0));
    properties->add(new GameEngine::Property<EnemyManagerComponent, float>(
            "mothershipFrequency",
            this,
            &EnemyManagerComponent::mothershipFrequency,
            &EnemyManagerComponent::mothershipFrequency,
            0));
    properties->add(new GameEngine::Property<EnemyManagerComponent, float>(
            "mothershipVariation",
            this,
            &EnemyManagerComponent::mothershipVariation,
            &EnemyManagerComponent::mothershipVariation,
            0));

    properties->add(new GameEngine::Property<EnemyManagerComponent, int>(
            "enemiesPerRow",
            this,
            &EnemyManagerComponent::enemiesPerRow,
            &EnemyManagerComponent::enemiesPerRow,
            0));

    properties->add(new GameEngine::Property<EnemyManagerComponent, std::vector<std::string>>(
            "rowsConfig",
            this,
            &EnemyManagerComponent::rowsConfig,
            &EnemyManagerComponent::rowsConfig,
            std::vector<std::string>()));


    properties->add(new GameEngine::Property<EnemyManagerComponent, GameEngine::Vec2D>(
            "margin",
            this,
            &EnemyManagerComponent::margins,
            &EnemyManagerComponent::margins,
            GameEngine::Vec2D()));
    return properties;
}

EnemyManagerComponent::~EnemyManagerComponent() {
    enemies_.clear();
}

void EnemyManagerComponent::enemyKilled(int EnemyKilledpoints) {
    currentSpeed_ *= 1.f + scaleFactor_;
    currentBulletFrequency_ *= 1.f - scaleFactor_;
    score_ += EnemyKilledpoints;
    notify(EnemyManagerEvents::ScoreChanged);
    checkMoveToNextLevel();
    updateBoundingBox();
}

void EnemyManagerComponent::updateBoundingBox() {

    GameEngine::Vec2D min(100000, 100000);
    GameEngine::Vec2D max;

    //recalculate the bounding box
    for(const GameEngine::geGameObjectRef &enemy : enemies_){
        if(!enemy->active())
            continue;

        if(min.x > enemy->position().x)
            min.x = enemy->position().x;

        if(max.x < enemy->position().x)
            max.x = enemy->position().x;


        if(min.y > enemy->position().y)
            min.y = enemy->position().y;

        if(max.y < enemy->position().y)
            max.y = enemy->position().y;
    }

    //move the calculated box to the world origin and add the width of the enemies
    min -= currentPosition_;
    max -= currentPosition_;
    if(auto firstEnemy = enemies_[0]->getComponent<GameEngine::SpriteAnimatedComponent>().lock()) {
        boundingBoxMin_ = GameEngine::Vec2D(min.x, min.y);
        boundingBoxMax_ = GameEngine::Vec2D(max.x + firstEnemy->getWidth(), max.y + firstEnemy->getHeight());
    }
}

void EnemyManagerComponent::checkMoveToNextLevel() {

    bool allEnemiesKilled = true;
    auto it = enemies_.begin();
    while (allEnemiesKilled && it != enemies_.end()){
        if((*it)->active())
            allEnemiesKilled = false;
        it++;
    }

    if(allEnemiesKilled){
        currentPosition_ = gameObject()->position();
        currentSpeed_ = std::abs(currentSpeed_);

        int rows = rowsConfig_.enemiesType.size();
        int columns = rowsConfig_.enemiesPerRow;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                const GameEngine::geGameObjectRef &alien = enemies_[i * columns + j];
                if(auto sprite = alien->getComponent<GameEngine::SpriteAnimatedComponent>().lock()) {
                    alien->position(currentPosition_ + GameEngine::Vec2D(
                            j * sprite->getWidth() + j * rowsConfig_.margins.x,
                            i * sprite->getHeight() + i * rowsConfig_.margins.y));
                }
                alien->active(true);
            }
        }

        for(const GameEngine::geGameObjectRef &bullet : bullets_){
            bullet->active(false);
        }

        if(auto mothership = mothership_.lock())
            mothership->hide();
    }
}

int EnemyManagerComponent::getScore() const {
    return score_;
}

void EnemyManagerComponent::speed(const float &speedValue){
    speed_ = speedValue;
}
float EnemyManagerComponent::speed() const{
    return speed_;
}

void EnemyManagerComponent::scaleFactor(const float &scaleValue){
    scaleFactor_ = scaleValue;
}
float EnemyManagerComponent::scaleFactor() const{
    return scaleFactor_;
}

void EnemyManagerComponent::bulletNum(const int &num){
    bulletsNum_ = num;
}
int EnemyManagerComponent::bulletNum() const{
    return bulletsNum_;
}

void EnemyManagerComponent::bulletFrequency(const float &frequency){
    bulletFrequency_ = frequency;
}
float EnemyManagerComponent::bulletFrequency() const{
    return bulletFrequency_;
}

void EnemyManagerComponent::mothershipFrequency(const float &frequency){
    mothershipFrequency_ = frequency;
}
float EnemyManagerComponent::mothershipFrequency() const{
    return mothershipFrequency_;
}

void EnemyManagerComponent::mothershipVariation(const float &variation){
    mothershipVariation_ = variation;
}
float EnemyManagerComponent::mothershipVariation() const{
    return mothershipVariation_;
}

void EnemyManagerComponent::enemiesPerRow(const int &num) {
    rowsConfig_.enemiesPerRow = num;
}

int EnemyManagerComponent::enemiesPerRow() const {
    return rowsConfig_.enemiesPerRow;
}

void EnemyManagerComponent::margins(const GameEngine::Vec2D &margin) {
    rowsConfig_.margins = margin;
}

GameEngine::Vec2D EnemyManagerComponent::margins() const {
    return rowsConfig_.margins;
}

void EnemyManagerComponent::rowsConfig(const std::vector<std::string> &config) {
    rowsConfig_.enemiesType = config;
}

std::vector<std::string> EnemyManagerComponent::rowsConfig() const {
    return rowsConfig_.enemiesType;
}
