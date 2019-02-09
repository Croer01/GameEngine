//
// Created by adria on 18/11/2018.
//

#ifndef SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
#define SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include "../graphics/Graphic.hpp"
#include "../graphics/GraphicHolder.hpp"
#include "../events/Observer.hpp"

namespace GameEngine {
namespace Internal {
        class SpriteAnimatedComponent : public geComponent, public Observer<GameObjectEvent> {
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
            void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
        };
    }
}

#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
