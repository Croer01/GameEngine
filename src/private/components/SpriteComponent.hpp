//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include "../Component.hpp"
#include "../graphics/Graphic.hpp"
#include "../graphics/GraphicHolder.hpp"
namespace GameEngine {
namespace Internal {
    class SpriteComponent : public Component, public Observer<GameObjectEvent> {
        std::shared_ptr<Graphic> graphicLoaded_;
        std::shared_ptr<GraphicHolder> graphic_;
        bool visible_;

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        ~SpriteComponent();

        void init() override;

        std::shared_ptr<Component> Clone() override;

        void fromFile(const YAML::Node &componentConfig) override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(bool visible);

        bool isVisible() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
    };
}
}

#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
