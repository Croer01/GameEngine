//
// Created by adria on 23/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP


#include <game-engine/components/ui/UITextComponent.hpp>
#include <game-engine/InputManager.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>

namespace GameEngine {
class PUBLICAPI UITextInputComponent : public UITextComponent, public Observer<InputTextSubjectEvent>
{
    const float BLINK_TIME_SECONDS = 0.5;
    float cursorBlinkCounter_;
    int cursorPos_;
    std::shared_ptr<Internal::GraphicHolder> graphicCursor_;
    std::shared_ptr<Internal::GraphicHolder> graphicBottomLine_;
    std::shared_ptr<Internal::GraphicHolder> backgroundGraphic_;
    geColor background_;
    float keyStepCounter_;

    void moveCursor(int movement);
    void createCursorGeometry();
    void createBottomLineGeometry();
    void updateBottomLineGeometry();
    void createBackgroundGraphic();
protected:
    geComponentRef instantiate() const override;
    virtual geComponentRef clone() const;
    void onFocusChanged() override;

    virtual void onVisibleChanged();

public:
    virtual ~UITextInputComponent();

    PropertySetBase *getProperties() const override;

    void Update(float elapsedTime) override;
    void init() override;
    void onEvent(const Subject<InputTextSubjectEvent> &target, const InputTextSubjectEvent &event, void *args) override;

    void background(const geColor &color);
    geColor background() const;
};
}


#endif //GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP
