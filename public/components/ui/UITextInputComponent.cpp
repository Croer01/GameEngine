//
// Created by adria on 23/09/2019.
//

#include <game-engine/InputManager.hpp>
#include <game-engine/components/ui/UITextInputComponent.hpp>
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

    return properties;
}

void UITextInputComponent::init()
{
    UITextComponent::init();
    inputFocus_ = false;
    moveCursor(0);
}

void UITextInputComponent::onClick()
{
    inputFocus_ = !inputFocus_;
    if(inputFocus_){
        InputTextSubjectRef subject = std::make_shared<InputTextSubject>();
        subject->registerObserver(this);
        InputManager::GetInstance().startRecordingTextInput(subject);
    }
    else
        InputManager::GetInstance().stopRecordingTextInput();
}

void UITextInputComponent::onEvent(const Subject<InputTextSubjectEvent> &target, const InputTextSubjectEvent &event,
                                   void *args)
{
    if(event == InputTextSubjectEvent::INPUT)
    {
        text(text().insert(cursorPos_, static_cast<char *>(args)));
        moveCursor(1);
    }
    else if(event == InputTextSubjectEvent::ERASE && cursorPos_ > 0)
    {
        text(text().erase(cursorPos_-1, 1));
        moveCursor(-1);
    }
}

void UITextInputComponent::Update(float elapsedTime)
{
    UIControlComponent::Update(elapsedTime);

    if(InputManager::GetInstance().isKeyDown(KeyCode::KEY_LEFT))
        moveCursor(-1);
    else if(InputManager::GetInstance().isKeyDown(KeyCode::KEY_RIGHT))
        moveCursor(1);
}

void UITextInputComponent::moveCursor(int movement)
{
    createCursorGeometry();
    cursorPos_ = std::max(0,std::min(static_cast<int>(text().size()),cursorPos_ + movement));
    Vec2D cursorPixelPos;
    if(cursorPos_ == text().size())
        cursorPixelPos = Vec2D(getTextSize().x,getGraphicText()->getPixelPosFromTextIndex(cursorPos_ -1).y);
    else
        cursorPixelPos = getGraphicText()->getPixelPosFromTextIndex(cursorPos_);
    graphicCursor_->setModelTransform(screenPos() + cursorPixelPos, Vec2D(0.f, 0.f), Vec2D(1,fontSize()));
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
}