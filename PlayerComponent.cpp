//
// Created by adria on 29/10/2018.
//

#include "PlayerComponent.hpp"
#include "src/InputManager.hpp"
#include "src/components/SpriteComponent.hpp"
#include "src/SceneManager.hpp"

std::shared_ptr<Component> PlayerComponent::Clone() {
    auto clone = std::make_shared<PlayerComponent>();
    clone->speed_ = speed_;
    clone->lives_ = lives_;
    return clone;
}

void PlayerComponent::fromFile(const YAML::Node &componentConfig) {
    speed_ = componentConfig["speed"].as<float>(0);
    lives_ = componentConfig["lives"].as<int>(0);
}

void PlayerComponent::init() {
    currentLives_ = lives_;
    bullet_ = SceneManager::GetInstance().createGameObject("PlayerBullet");
    bullet_->setName("PlayerBullet");
    shootSound_ = parent_->getComponent<AudioComponent>();
    lifesCounter_ = SceneManager::GetInstance()
            .findObjectByName("LifesCounter")->
            findChildByName("Label")->
            getComponent<TextComponent>();
    if(auto lifesCounter = lifesCounter_.lock())
        lifesCounter->setText("X" + std::to_string(currentLives_));
}

void PlayerComponent::Update(float elapsedTime) {
    glm::vec3 translation(0);
    auto sprite = parent_->getComponent<SpriteComponent>().lock();

    if (InputManager::GetInstance().isKeyPressed(SDLK_RIGHT)) {
        translation.x+=speed_;
    } else if (InputManager::GetInstance().isKeyPressed(SDLK_LEFT)) {
        translation.x-=speed_;
    }

    if(translation.x != 0){
        glm::vec3 desiredPos = parent_->getPosition() + (translation * elapsedTime);

        if(desiredPos[0] < 0)
            desiredPos[0] = 0;
        else if(desiredPos[0] + sprite->getWidth() > 224)
            desiredPos[0] = 224 - sprite->getWidth();

        parent_->setPosition(desiredPos);
    }

    if(!bullet_->isActive() && InputManager::GetInstance().isKeyPressed(SDLK_SPACE)){
        bullet_->setPosition(parent_->getPosition() + glm::vec3(sprite->getWidth()/2, 0, 0));
        bullet_->setActive(true);
        if(auto shootSound = shootSound_.lock())
            shootSound->play();
    }
}

void PlayerComponent::kill() {
    currentLives_--;
    if(currentLives_ < 0) {
        parent_->setActive(false);
        SceneManager::GetInstance().changeScene("StartMenu");
    } else {
        if(auto lifesCounter = lifesCounter_.lock())
            lifesCounter->setText("X" + std::to_string(currentLives_));
    }
}
