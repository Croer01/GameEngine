//
// Created by adria on 04/11/2018.
//

#ifndef SPACEINVADERS_TEXTCOMPONENT_HPP
#define SPACEINVADERS_TEXTCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include "../graphics/font/Text.hpp"
#include "../events/Observer.hpp"

namespace GameEngine {
namespace Internal {
    class TextComponent : public geComponent, public Observer<GameObjectEvent> {
        std::string text_;
        std::string font_;
        unsigned int fontSize_;
        std::shared_ptr<Text> textGraphic_;
        bool visible_;

        void updateTextTransform();

    public:
        virtual ~TextComponent();

        void init() override;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        //Text public API
        void setText(const std::string &text);

        void setVisible(bool visible);

        bool isVisible() const;

    protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}
}

#endif //SPACEINVADERS_TEXTCOMPONENT_HPP
