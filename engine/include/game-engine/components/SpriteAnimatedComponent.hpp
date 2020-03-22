//
// Created by adria on 18/11/2018.
//

#ifndef SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
#define SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Subject.hpp>
#include "../private/graphics/Graphic.hpp"
#include "../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
    class PUBLICAPI SpriteAnimatedComponent : public geComponentInstantiable<SpriteAnimatedComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        int columns_ = 1;
        int rows_ = 1;
        bool visible_;
        bool playing_;
        std::string filePath_;
        geColor color_;
        std::string anchor_;
        // animation variables
        float timePerFrame_;
        float timeAcumulator_;
        int animationOffsetFrame_;
        int animationFramesLength_;
        int animationCurrentFrame_;

        void resetAnimation();
        void updateGraphicRef();

    protected:
        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        ~SpriteAnimatedComponent();

        PropertySetBase *getProperties() const override;

        void init() override;

        void Update(float elapsedTime) override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void play();

        void playRange(int firstFrame, int length);

        bool isPlaying() const;

        void pause();

        void stop();

        void setFrame(int frame);

        int getFramesNum() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);
        std::string filepath() const;

        void rows(const int &numRows);
        int rows() const;

        void columns(const int &numColumns);
        int columns() const;

        void color(const geColor &value);
        geColor color() const;

        float framesPerSecond() const;
        void framesPerSecond(const float &frames);

        bool playOnInit() const;
        void playOnInit(const bool &play);

        void anchor(const std::string &anchor);
        std::string anchor() const;
    };
}

#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
