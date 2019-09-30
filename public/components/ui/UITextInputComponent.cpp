//
// Created by adria on 23/09/2019.
//

#include <game-engine/InputManager.hpp>
#include <game-engine/components/ui/UITextInputComponent.hpp>
#include <utf8.h>
#include "../../../private/graphics/GraphicGeometry.hpp"
#include "../../../private/graphics/GraphicsEngine.hpp"

namespace GameEngine {

geComponentRef UITextInputComponent::instantiate() const
{
    return std::make_shared<UITextInputComponent>();
}

PropertySetBase *UITextInputComponent::configureProperties()
{
    auto *base = UITextComponent::configureProperties();
    auto *properties = new PropertySet<UITextInputComponent>(this, base);

    properties->add(new Property<UITextInputComponent, geColor>(
        "background",
        this,
        &UITextInputComponent::background,
        &UITextInputComponent::background,
        geColor(1.f)
    ));

    return properties;
}

void UITextInputComponent::init()
{
    UITextComponent::init();
    createBackgroundGraphic();
    backgroundGraphic_->setTintColor(background_);

    moveCursor(0);
    keyStepCounter_ = 0.f;
    graphicCursor_->setActive(false);
    cursorBlinkCounter_ = BLINK_TIME_SECONDS;

    createBottomLineGeometry();
    updateBottomLineGeometry();
}

void UITextInputComponent::onEvent(const Subject<InputTextSubjectEvent> &target, const InputTextSubjectEvent &event,
                                   void *args)
{
    if(event == InputTextSubjectEvent::INPUT)
    {
        // calculate the Unicode length to set the new char
        std::string tmpText = text();
        auto it = tmpText.begin();
        int i = 0;
        while(i < cursorPos_)
        {
            utf8::next(it, tmpText.end());
            i++;
        }
        tmpText.insert(it - tmpText.begin(), std::string(static_cast<char *>(args)));
        text(tmpText);
        moveCursor(1);
    }
    else if(event == InputTextSubjectEvent::ERASE && cursorPos_ > 0)
    {
        // calculate the Unicode length to erase the char in the cursor position
        std::string tmpText = text();
        auto it = tmpText.begin();
        int i = 0;
        while(i < cursorPos_ - 1)
        {
            utf8::next(it, tmpText.end());
            i++;
        }

        // calculate the Unicode size of the char to remove from the cursor position
        auto itEnd = it;
        utf8::next(itEnd,tmpText.end());
        text(tmpText.erase(it - tmpText.begin(), itEnd - it));
        moveCursor(-1);
    }
    // always update the bottom line
    updateBottomLineGeometry();
}

void UITextInputComponent::Update(float elapsedTime)
{
    constexpr float KEY_STEP_TIME_SECONDS = 0.06f;
    UIControlComponent::Update(elapsedTime);

    if(isFocused())
    {
        if(keyStepCounter_ <= 0.f)
        {
            if (InputManager::GetInstance().isKeyPressed(KeyCode::KEY_LEFT))
            {
                moveCursor(-1);
                keyStepCounter_ = KEY_STEP_TIME_SECONDS;

                // Ensure the cursor is visible after it is moved
                graphicCursor_->setActive(true);
                cursorBlinkCounter_ = BLINK_TIME_SECONDS;
            }
            else if (InputManager::GetInstance().isKeyPressed(KeyCode::KEY_RIGHT))
            {
                moveCursor(1);
                keyStepCounter_ = KEY_STEP_TIME_SECONDS;

                // Ensure the cursor is visible after it is moved
                graphicCursor_->setActive(true);
                cursorBlinkCounter_ = BLINK_TIME_SECONDS;
            }
        } else{
            keyStepCounter_ -= elapsedTime;
        }

        if(cursorBlinkCounter_ <= 0.f)
        {
            graphicCursor_->setActive(!graphicCursor_->getActive());
            cursorBlinkCounter_ = BLINK_TIME_SECONDS;
        }

        cursorBlinkCounter_ -= elapsedTime;
    }
}

void UITextInputComponent::moveCursor(int movement)
{
    createCursorGeometry();
    cursorPos_ = std::max(0,std::min(static_cast<int>(getGraphicText()->size()), cursorPos_ + movement));
    Vec2D cursorPixelPos;

    cursorPixelPos = getGraphicText()->getPixelPosFromTextIndex(cursorPos_ - 1);
    cursorPixelPos.x += getGraphicText()->getPixelSizeFromTextIndex(cursorPos_ - 1).x;

    graphicCursor_->setModelTransform(screenPos() + cursorPixelPos, Vec2D(0.f, 0.f), Vec2D(1,fontSize()));
}

void UITextInputComponent::createBottomLineGeometry()
{
    if(graphicBottomLine_)
        return;

    std::vector<Vec2D> path = {
        Vec2D(.0f, .0f),
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    auto graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
    graphicBottomLine_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    graphicBottomLine_->setModelTransform(Vec2D(0,0), Vec2D(0.f, 0.f), Vec2D(1,1));
    Internal::GraphicsEngine::GetInstance().registerGraphic(graphicBottomLine_);
}

void UITextInputComponent::updateBottomLineGeometry()
{
    const std::shared_ptr<Internal::Text> &textGraphic = getGraphicText();
    graphicBottomLine_->setModelTransform(screenPos() + Vec2D(0, textGraphic->height()), Vec2D(0.f, 0.f), Vec2D(textGraphic->width(), 1));
}

void UITextInputComponent::createCursorGeometry()
{
    if(graphicCursor_)
        return;

    std::vector<Vec2D> path = {
        Vec2D(.0f, .0f),
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    auto graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
    graphicCursor_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    graphicCursor_->setModelTransform(Vec2D(0,0), Vec2D(0.f, 0.f), Vec2D(1,fontSize()));
    Internal::GraphicsEngine::GetInstance().registerGraphic(graphicCursor_);
}

UITextInputComponent::~UITextInputComponent()
{
    if(graphicCursor_)
        Internal::GraphicsEngine::GetInstance().unregisterGraphic(graphicCursor_);
}

void UITextInputComponent::onFocusChanged()
{
    if(isFocused())
    {
        InputTextSubjectRef subject = std::make_shared<InputTextSubject>();
        subject->registerObserver(this);
        InputManager::GetInstance().startRecordingTextInput(subject);
        graphicCursor_->setActive(true);
    }
    else
    {
        InputManager::GetInstance().stopRecordingTextInput();
        graphicCursor_->setActive(false);
        moveCursor(-cursorPos_);
        keyStepCounter_ = 0;
        cursorBlinkCounter_ = BLINK_TIME_SECONDS;
    }
}

void UITextInputComponent::createBackgroundGraphic()
{
    if(backgroundGraphic_)
        return;

    std::vector<Vec2D> path = {
        Vec2D(.0f, .0f),
        Vec2D(1.f, .0f),
        Vec2D(1.f, 1.f),
        Vec2D(.0f, 1.f)
    };

    auto graphicLoaded_ = std::make_shared<Internal::GraphicGeometry>(path);
    backgroundGraphic_ = std::make_shared<Internal::GraphicHolder>(graphicLoaded_);
    backgroundGraphic_->setModelTransform(screenPos(), Vec2D(0.f, 0.f), screenSize());
    Internal::GraphicsEngine::GetInstance().registerGraphic(backgroundGraphic_);
}

void UITextInputComponent::background(const geColor &color)
{
    background_ = color;
}

geColor UITextInputComponent::background() const
{
    return background_;
}
}