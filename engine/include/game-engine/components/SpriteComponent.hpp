//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include <game-engine/internal/graphics/Graphic.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine {

    class PUBLICAPI SpriteComponent
            : public geComponentInstantiable<SpriteComponent>, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        bool visible_;
        std::string filePath_;
        std::string anchor_;
        Vec2D displacement_;

        Internal::GraphicsEngine *graphicsEngine_;

        void updateGraphicRef();

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;

    public:
        virtual ~SpriteComponent();

        PropertySetBase *getProperties() const override;

        virtual void preInit();

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);

        std::string filepath() const;

        void anchor(const std::string &anchor);

        std::string anchor() const;

        void displacement(const Vec2D &value);
        Vec2D displacement() const;
    };
}

#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
