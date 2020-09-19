//
// Created by adria on 18/09/2020.
//

#include <iostream>
#include "GameEditor.hpp"

GameEditor::GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment) : Game(environment)
{
    environment->registerComponent("GameEditorComponent", new GameEngine::ComponentTBuilder<GameEditorComponent>());

}

GameEngine::geGameObjectRef GameEditor::createFromPrototype(const std::string &prototype)
{
    const GameEngine::geGameObjectRef &obj = Game::createFromPrototype(prototype);
    auto editorComponent = std::make_shared<GameEditorComponent>();
    obj->addComponent(editorComponent);
    components_.push_back(editorComponent);
    return obj;
}

GameEngine::geGameObjectRef GameEditor::createObject(const std::string &name)
{
    const std::shared_ptr<GameEngine::Internal::GameObject> &object = std::make_shared<GameEngine::Internal::GameObject>("");
    object->name(name);
    object->game(this);
    if(environment_->sceneManager()->isSceneLoaded())
        environment_->sceneManager()->addObjectIntoCurrentScene(object);

    auto editorComponent = std::make_shared<GameEditorComponent>();
    object->addComponent(editorComponent);
    components_.push_back(editorComponent);
    return object;
}

void GameEditor::update()
{
    assert(initialized_);
    screen_->update();

    inputManager_->update();
    if (inputManager_->isQuitDown()) {
        shutdown();
        return;
    }

    timeManager_->calculateTime();
    if(timeManager_->isFrameSkipped())
        return;
    float elapsedTime = timeManager_->getElapsedTime();
    environment_->sceneManager()->changeSceneInSafeMode();

#ifdef DEBUG
    // This is a dirty solution to avoid big elapsed times when debugging.
    // A common situation is when the execution is stopped a long time by a breakpoint.
    if (elapsedTime >= 5)
        elapsedTime = 1.f / 60.f;
#endif

    // This is the unique components we want to update in the editor
    for (const auto& component : components_)
    {
        if(auto c = component.lock())
            c->Update(elapsedTime);
    }
}

void GameEditor::changeScene(const std::string &name)
{
    components_.clear();
    Game::changeScene(name);
}

GameEngine::PropertySetBase *GameEditorComponent::getProperties() const
{
    return new GameEngine::PropertySet<GameEditorComponent>();
}

void GameEditorComponent::Update(float elapsedTime)
{
    GameEngine::InputManager *input = gameObject()->game()->input();

    const GameEngine::Vec2D &mousePosition = input->getMousePosition();

    std::cout << "x=" << mousePosition.x << " y=" << mousePosition.y << std::endl;
    GameEngine::Vec2D mouseDistance = gameObject()->position() - mousePosition;

    //std::cout << gameObject()->name() << " " << mouseDistance.magnitude() << std::endl;

    if(drag_)
    {
       gameObject()->position(gameObject()->position() + mousePosition - lastMousePos_);
    }
    else if(std::abs(mouseDistance.sqrMagnitude()) <= 50 * 50)
    {
        if(input->isMouseButtonDown(GameEngine::MouseButton::LEFT))
        {
            drag_ = true;
        }
    }

    if(input->isMouseButtonUp(GameEngine::MouseButton::LEFT))
    {
        drag_ = false;
    }

    lastMousePos_ = mousePosition;
}

void GameEditorComponent::init()
{
    drag_ = false;
}
