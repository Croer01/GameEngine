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
#include <game-engine/properties/PropertiesRegister.hpp>

namespace GameEngine {
    class PUBLICAPI SpriteAnimatedComponent : public geComponentInstantiable<SpriteAnimatedComponent>, public Observer<GameObjectEvent> {
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
        void updateGraphicRef();

    protected:
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

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);

        std::string filepath() const;

        void rows(const int &numRows);

        int rows() const;

        void columns(const int &numColumns);

        int columns() const;

        float framesPerSecond() const;
        void framesPerSecond(const float &frames);

        bool playOnInit() const;
        void playOnInit(const bool &play);
    };

template <>
class PropertyInstantiator<SpriteAnimatedComponent>
{
public:
    static std::string getTargetName()
    {
        return "SpriteAnimatedComponent";
    }

    static PropertySetBase* instantiate()
    {
        auto *properties = new PropertySet<SpriteAnimatedComponent>();

        properties->add(new Property<SpriteAnimatedComponent, std::string>(
            "filePath",
            &SpriteAnimatedComponent::filepath,
            &SpriteAnimatedComponent::filepath,
            "",
            true));

        properties->add(new Property<SpriteAnimatedComponent, int>(
            "rows",
            &SpriteAnimatedComponent::rows,
            &SpriteAnimatedComponent::rows,
            1));

        properties->add(new Property<SpriteAnimatedComponent, int>(
            "columns",
            &SpriteAnimatedComponent::columns,
            &SpriteAnimatedComponent::columns,
            1));

        //get the time between frames. default is 1 sec.
        properties->add(new Property<SpriteAnimatedComponent, float>(
            "framesPerSecond",
            &SpriteAnimatedComponent::framesPerSecond,
            &SpriteAnimatedComponent::framesPerSecond,
            1.f));

        properties->add(new Property<SpriteAnimatedComponent, bool>(
            "visible",
            &SpriteAnimatedComponent::isVisible,
            &SpriteAnimatedComponent::setVisible,
            true));

        properties->add(new Property<SpriteAnimatedComponent, bool>(
            "playOnInit",
            &SpriteAnimatedComponent::playOnInit,
            &SpriteAnimatedComponent::playOnInit,
            true));

        return properties;
    }
};
}

#endif //SPACEINVADERS_SPRITEANIMATEDCOMPONENT_HPP
