//
// Created by adria on 23/09/2019.
//

#ifndef GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP
#define GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP


#include <game-engine/components/ui/UITextComponent.hpp>
#include "../../../../private/graphics/GraphicHolder.hpp"
#include <game-engine/properties/PropertiesRegister.hpp>

namespace GameEngine {
class PUBLICAPI UITextInputComponent : public UITextComponent, public Observer<InputTextSubjectEvent>
{
    static PropertyInstantiator<UITextInputComponent> registrator_;

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
    void onFocusChanged() override;
public:
    virtual ~UITextInputComponent();

    void Update(float elapsedTime) override;
    void init() override;
    void onEvent(const Subject<InputTextSubjectEvent> &target, const InputTextSubjectEvent &event, void *args) override;

    void background(const geColor &color);
    geColor background() const;
};

template <>
class PropertyInstantiator<UITextInputComponent>
{
public:
    static std::string getTargetName()
    {
        return "UITextInputComponent";
    }
    static PropertySetBase* instantiate()
    {
        auto *base = PropertyInstantiator<UITextComponent>::instantiate();
        auto *properties = new PropertySet<UITextInputComponent>(base);

        properties->add(new Property<UITextInputComponent, geColor>(
            "background",
            &UITextInputComponent::background,
            &UITextInputComponent::background,
            geColor(1.f)
        ));

        return properties;
    }
};
}


#endif //GAMEENGINEEDITOR_UITEXTINPUTCOMPONENT_HPP
