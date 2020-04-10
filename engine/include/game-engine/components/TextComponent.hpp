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
class PUBLICAPI TextComponent : public geComponentInstantiable<TextComponent>, public Observer<GameObjectEvent> {
        Internal::TextParameters textParams_;
        std::shared_ptr<Internal::Text> textGraphic_;
        std::shared_ptr<Internal::GraphicsEngine> graphicsEngine_;
        bool visible_;

        void updateTextTransform();

        void updateTextRef();

    public:
        virtual ~TextComponent();

        PropertySetBase *getProperties() const override;

        void preInit() override;
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
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}

#endif //SPACEINVADERS_TEXTCOMPONENT_HPP
