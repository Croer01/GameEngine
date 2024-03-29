//
// Created by adria on 23/09/2019.
//

#include <game-engine/components/ui/UITextInputComponent.hpp>
#include <utf8.h>
#include <game-engine/internal/graphics/GraphicGeometry.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {

geComponentRef UITextInputComponent::instantiate() const
{
    return std::make_shared<UITextInputComponent>();
}

geComponentRef UITextInputComponent::clone() const
{
    geComponentRef cloned = instantiate();
    cloned->setData(getData()->template clone<UITextInputComponentData>());
    return cloned;
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

void UITextInputComponent::onEvent(const Subject<InputTextSubjectEvent, const char *> &target, InputTextSubjectEvent event, const char *textInput)
{
    if(event == InputTextSubjectEvent::INPUT)
    {
        // calculate the Unicode length to set the new char
        std::string tmpText = getPropertyValue<std::string>("text");
        auto it = tmpText.begin();
        int i = 0;
        while(i < cursorPos_)
        {
            utf8::next(it, tmpText.end());
            i++;
        }
        tmpText.insert(it - tmpText.begin(), std::string(textInput));
        setPropertyValue<std::string>("text", tmpText);
        moveCursor(1);
    }
    else if(event == InputTextSubjectEvent::ERASE && cursorPos_ > 0)
    {
        // calculate the Unicode length to erase the char in the cursor position
        std::string tmpText = getPropertyValue<std::string>("text");
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
        setPropertyValue<std::string>("text", tmpText.erase(it - tmpText.begin(), itEnd - it));
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
            InputManager *input = gameObject()->game()->input();
            if (input->isKeyPressed(KeyCode::KEY_LEFT))
            {
                moveCursor(-1);
                keyStepCounter_ = KEY_STEP_TIME_SECONDS;

                // Ensure the cursor is visible after it is moved
                graphicCursor_->setActive(true);
                cursorBlinkCounter_ = BLINK_TIME_SECONDS;
            }
            else if (input->isKeyPressed(KeyCode::KEY_RIGHT))
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

    int fontSize = getPropertyValue<int>("fontSize");
    graphicCursor_->setModelTransform(calculateVirtualScreenPos() + cursorPixelPos, 0.f, Vec2D(1, fontSize));
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
    graphicBottomLine_->setModelTransform(Vec2D(0,0), 0.f, Vec2D(1,1));
    graphicsEngine()->registerGraphic(graphicBottomLine_);
    graphicBottomLine_->setActive(getPropertyValue<bool>("visible"));
}

void UITextInputComponent::updateBottomLineGeometry()
{
    const std::shared_ptr<Internal::Text> &textGraphic = getGraphicText();
    graphicBottomLine_->setModelTransform(calculateVirtualScreenPos() + Vec2D(0, textGraphic->height()), 0.f, Vec2D(textGraphic->width(), 1));
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
    int fontSize = getPropertyValue<int>("fontSize");
    graphicCursor_->setModelTransform(Vec2D(0,0), 0.f, Vec2D(1,fontSize));
    graphicsEngine()->registerGraphic(graphicCursor_);
    graphicCursor_->setActive(getPropertyValue<bool>("visible"));
}

UITextInputComponent::~UITextInputComponent()
{
    if(graphicCursor_)
        graphicsEngine()->unregisterGraphic(graphicCursor_);
    if(graphicBottomLine_)
        graphicsEngine()->unregisterGraphic(graphicBottomLine_);
}

void UITextInputComponent::onFocusChanged()
{
    if(!getPropertyValue<bool>("visible"))
        return;

    if(isFocused())
    {
        InputTextSubjectRef subject = std::make_shared<InputTextSubject>();
        subject->registerObserver(this);
        gameObject()->game()->input()->startRecordingTextInput(subject);
        graphicCursor_->setActive(true);
    }
    else
    {
        gameObject()->game()->input()->stopRecordingTextInput();
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
    backgroundGraphic_->setModelTransform(calculateVirtualScreenPos(), 0.f, calculateVirtualScreenSize());
    graphicsEngine()->registerGraphic(backgroundGraphic_);
    backgroundGraphic_->setActive(getPropertyValue<bool>("visible"));
}

void UITextInputComponent::onVisibleChanged()
{
    UITextComponent::onVisibleChanged();
    bool visible = getPropertyValue<bool>("visible");

    if(graphicBottomLine_)
        graphicBottomLine_->setActive(visible);
    if(graphicCursor_)
        graphicCursor_->setActive(visible);
    if(backgroundGraphic_)
        backgroundGraphic_->setActive(visible);
}

ComponentDataRef UITextInputComponent::instantiateData() const
{
    return std::make_shared<UITextInputComponentData>();
}
}