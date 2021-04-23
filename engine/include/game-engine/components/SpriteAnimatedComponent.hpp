//
// Created by adria on 18/11/2018.
//

#ifndef SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
#define SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Subject.hpp>
#include <game-engine/internal/graphics/Graphic.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {
class SpriteAnimatedComponentData : public ComponentData
{
public:
    SpriteAnimatedComponentData()
    {
        createPropertyFilePath("filePath", "", FileType::IMAGE, true);
        createProperty<geColor>("tint", geColor(1.f));
        createProperty<int>("rows", 1);
        createProperty<int>("columns", 1);
        createProperty<float>("framesPerSecond", 1.f);
        createProperty<bool>("visible", true);
        createProperty<bool>("playOnInit", true);
        createPropertyEnum("anchor", "TOP_LEFT", {
            "TOP_LEFT",
            "TOP_CENTER",
            "TOP_RIGHT",
            "MIDDLE_LEFT",
            "MIDDLE_CENTER",
            "MIDDLE_RIGHT",
            "BOTTOM_LEFT",
            "BOTTOM_CENTER",
            "BOTTOM_RIGHT"
        });
    }
};
    class PUBLICAPI SpriteAnimatedComponent : public geComponentInstantiable<SpriteAnimatedComponent, SpriteAnimatedComponentData>,
        public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        Internal::GraphicsEngine *graphicsEngine_;

        bool playing_;
        // animation variables
        float timePerFrame_;
        float timeAcumulator_;
        int animationOffsetFrame_;
        int animationFramesLength_;
        int animationCurrentFrame_;

        void resetAnimation();
        void updateGraphicRef();
        void updateVisible();
        void updateTint();
        void updateAnchor();
        void updateGrid();
        void updateTimePerSecond();

    protected:
        void
        onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~SpriteAnimatedComponent();

        virtual void preInit();

        void Update(float elapsedTime) override;

        int getWidth() const;

        int getHeight() const;

        void play();

        void playRange(int firstFrame, int length);

        bool isPlaying() const;

        void pause();

        void stop();

        void setFrame(int frame);

        int getFramesNum() const;

        void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;
    };
}

#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
