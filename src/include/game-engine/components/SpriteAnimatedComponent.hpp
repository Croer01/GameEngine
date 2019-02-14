//
// Created by adria on 18/11/2018.
//

#ifndef SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
#define SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include "../private/graphics/Graphic.hpp"
#include "../private/graphics/GraphicHolder.hpp"
#include "../private/events/Observer.hpp"

namespace GameEngine {
    class PUBLICAPI SpriteAnimatedComponent : public geComponent, public Internal::Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        float timePerFrame_;
        float timeAcumulator_;
        int columns_;
        int rows_;
        int index_[2];
        bool visible_;
        bool playing_;
        std::string filePath_;

        void resetAnimation();

    protected:
        PropertySetBase *configureProperties() override;

        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        ~SpriteAnimatedComponent();

        void init() override;

        void Update(float elapsedTime) override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void play();

        bool isPlaying() const;

        void stop();

        void setFrame(int frame);

        int getFramesNum() const;

        void onEvent(const Internal::Subject <GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);

        std::string filepath() const;

        void rows(const int &numRows);

        int rows() const;

        void columns(const int &numColumns);

        int columns() const;

        void framesPerSecond(const float &frames);

        void playOnInit(const bool &play);
    };
}

#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
