//
// Created by adria on 04/11/2018.
//

#ifndef SPACEINVADERS_TEXTCOMPONENT_HPP
#define SPACEINVADERS_TEXTCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/events/Observer.hpp>
#include <game-engine/internal/graphics/font/Text.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {

class TextComponentData : public ComponentData
{
public:
    TextComponentData()
    {
        createProperty<std::string>("font", "", true);
        createProperty<int>("fontSize", 0);
        createProperty<std::string>("text", "");
    }
};

class PUBLICAPI TextComponent : public geComponentInstantiable<TextComponent, TextComponentData>, public Observer<GameObjectEvent> {
        Internal::TextParameters textParams_;
        std::shared_ptr<Internal::Text> textGraphic_;
        Internal::GraphicsEngine *graphicsEngine_;
        bool visible_;

        void updateTextTransform();
        void updateFont();
        void updateFontSize();
        void updateText();

    void updateTextRef();

    public:
        virtual ~TextComponent();

        void preInit() override;
        void init() override;

        void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent event) override;

        void setVisible(bool visible);

        bool isVisible() const;
protected:
        void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) override;
    };
}

#endif //SPACEINVADERS_TEXTCOMPONENT_HPP
