//
// Created by adria on 08/10/2018.
//

#ifndef SPACEINVADERS_SPRITECOMPONENT_HPP
#define SPACEINVADERS_SPRITECOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include "../private/graphics/Graphic.hpp"
#include "../private/graphics/GraphicHolder.hpp"

namespace GameEngine {
class PUBLICAPI SpriteComponent : public geComponent, public Observer<GameObjectEvent> {
        std::shared_ptr<Internal::Graphic> graphicLoaded_;
        std::shared_ptr<Internal::GraphicHolder> graphic_;
        bool visible_;
        std::string filePath_;

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
        virtual PropertySetBase *configureProperties() override;

    geComponentRef instantiate() const override;

public:
        virtual ~SpriteComponent();

        void init() override;

        int getWidth() const;

        int getHeight() const;

        void setVisible(const bool &visible);

        bool isVisible() const;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void filepath(const std::string &path);
        std::string filepath() const;
    };
}

#endif //SPACEINVADERS_SPRITECOMPONENT_HPP
