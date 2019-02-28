//
// Created by adria on 04/11/2018.
//

#ifndef SPACEINVADERS_TEXTCOMPONENT_HPP
#define SPACEINVADERS_TEXTCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include "../private/graphics/font/Text.hpp"

namespace GameEngine {
class TextComponent : public geComponent, public Observer<GameObjectEvent> {
        std::string text_;
        std::string font_;
        unsigned int fontSize_;
        std::shared_ptr<Internal::Text> textGraphic_;
        bool visible_;

        void updateTextTransform();

    public:
        virtual ~TextComponent();

        void init() override;

        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        //Text public API
        int fontSize() const;
        void fontSize(const int &size);

        std::string font() const;
        void font(const std::string &fontName);

        void text(const std::string &text);
        std::string text() const;

        void setVisible(bool visible);

        bool isVisible() const;

protected:
    PropertySetBase *configureProperties() override;

    geComponentRef instantiate() const override;

protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}

#endif //SPACEINVADERS_TEXTCOMPONENT_HPP
