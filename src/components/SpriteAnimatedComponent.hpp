//
// Created by adria on 18/11/2018.
//

#ifndef SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
#define SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP


#include "../Component.hpp"
#include "../graphics/Graphic.hpp"
#include "../graphics/GraphicHolder.hpp"

class SpriteAnimatedComponent : public Component, public Observer<GameObjectEvent>{
    std::shared_ptr<Graphic> graphicLoaded_;
    std::shared_ptr<GraphicHolder> graphic_;
    float timePerFrame_;
    float timeAcumulator_;
    int columns_;
    int rows_;
    int index_[2];
    bool visible_;
    bool playing_;

    void resetAnimation();

public:
    ~SpriteAnimatedComponent();

    void init() override;

    std::shared_ptr<Component> Clone() override;

    void fromFile(const YAML::Node &componentConfig) override;

    void Update(float elapsedTime) override;

    int getWidth() const;
    int getHeight() const;
    void setVisible(bool visible);
    bool isVisible() const;

    void play();
    bool isPlaying() const;
    void stop();
    void setFrame(int frame);
    int getFramesNum() const;

    void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

protected:
    void onGameObjectChange(GameObject *oldGameObject, GameObject *newGameObject) override;
};


#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
