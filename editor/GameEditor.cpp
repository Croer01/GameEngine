//
// Created by adria on 18/09/2020.
//

#include <iostream>
#include "GameEditor.hpp"

GameEditor::GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment) : Game(environment)
{
    dirty_ = false;
    environment->registerComponent("GameEditorComponent", new GameEngine::ComponentTBuilder<GameEditorComponent>());
}

GameEngine::geGameObjectRef GameEditor::createFromPrototype(const std::string &prototype)
{
    const GameEngine::geGameObjectRef &obj = Game::createFromPrototype(prototype);
    auto editorComponent = std::make_shared<GameEditorComponent>();
    addEditorComponent(obj);
    return obj;
}

GameEngine::geGameObjectRef GameEditor::createObject(const std::string &name)
{
    const std::shared_ptr<GameEngine::Internal::GameObject> &object = std::make_shared<GameEngine::Internal::GameObject>("");
    object->name(name);
    object->game(this);
    if(environment_->sceneManager()->isSceneLoaded())
        environment_->sceneManager()->addObjectIntoCurrentScene(object);

    addEditorComponent(object);
    return object;
}

void GameEditor::addEditorComponent(const GameEngine::geGameObjectRef &object)
{
    auto editorComponent = std::make_shared<GameEditorComponent>();
    object->addComponent(editorComponent);
    components_.push_back(editorComponent);
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

    environment_->sceneManager()->removeDestroyedObjects();
}

void GameEditor::changeScene(const std::string &name)
{
    components_.clear();
    Game::changeScene(name);
}

void GameEditor::linkSceneDataWithCurrentScene()
{
    for(const auto &component : components_)
    {
        if(auto comp = component.lock())
        {
            assert(sceneData_);
            std::string name = comp->gameObject()->name();
            auto it = std::find_if(sceneData_->getObjectsBegin(), sceneData_->getObjectsEnd(),
                                   [name](auto object) { return object->name_ == name; });
            if(it != sceneData_->getObjectsEnd())
            {
                comp->linkObject(it->get());
            }
        }
    }
}
void GameEditor::linkSceneFromEditor(const SceneDataRef &scene)
{
    sceneData_ = scene;
    sceneData_->registerObserver(this);
}

void GameEditor::init()
{
    Game::init();
    linkSceneDataWithCurrentScene();
}

void GameEditor::setDirty(bool value)
{
    dirty_ = value;
}

bool GameEditor::isDirty() const
{
    return dirty_;
}

GameEditor::~GameEditor()
{
    if(sceneData_)
        sceneData_->unregisterObserver(this);
}

void GameEditor::onEvent(const GameEngine::Subject<SceneDataEvent> &target, const SceneDataEvent &event, void *args)
{
    if(event == SceneDataEvent::ObjectAdded)
    {
        auto prototypeRef = static_cast<PrototypeReference*>(args);
        const GameEngine::geGameObjectRef &object = createFromPrototype(prototypeRef->prototype_);
        object->name(prototypeRef->name_);
        object->position(GameEngine::Vec2D(prototypeRef->position_.xy[0],prototypeRef->position_.xy[1]));
        object->scale(GameEngine::Vec2D(prototypeRef->scale_.xy[0],prototypeRef->scale_.xy[1]));
        object->rotation(prototypeRef->rotation_);

        if(environment_->sceneManager()->isSceneLoaded())
        {
            environment_->sceneManager()->addObjectIntoCurrentScene(std::dynamic_pointer_cast<GameEngine::Internal::GameObject>(object));
            if(auto comp = object->getComponent<GameEditorComponent>().lock())
            {
                comp->linkObject(prototypeRef);
            }
        }
    }
    else if(event == SceneDataEvent::ObjectDeleted)
    {
        auto prototypeRef = static_cast<PrototypeReference*>(args);
        const GameEngine::geGameObjectRef &object = findObjectByNameInCurrentScene(prototypeRef->name_);
        assert(object);
        object->destroy();
    }
}

GameEngine::PropertySetBase *GameEditorComponent::getProperties() const
{
    return new GameEngine::PropertySet<GameEditorComponent>();
}

void GameEditorComponent::Update(float elapsedTime)
{
    GameEngine::InputManager *input = gameObject()->game()->input();

    const GameEngine::Vec2D &mousePosition = input->getMousePosition();

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
    gameObject()->registerObserver(this);
}

void GameEditorComponent::linkObject(PrototypeReference *data)
{
    data_ = data;
}

void GameEditorComponent::onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target,
                                  const GameEngine::GameObjectEvent &event, void *args)
{
    if(data_)
    {
        switch(event)
        {
            case GameEngine::GameObjectEvent::PositionChanged:
                data_->position_.xy[0] = gameObject()->position().x;
                data_->position_.xy[1] = gameObject()->position().y;
                break;
            case GameEngine::GameObjectEvent::RotationChanged:
                data_->rotation_ = gameObject()->rotation();
                break;
            case GameEngine::GameObjectEvent::ScaleChanged:
                data_->scale_.xy[0] = gameObject()->scale().x;
                data_->scale_.xy[1] = gameObject()->scale().y;
                break;
            case GameEngine::GameObjectEvent::ActiveChanged:
                break;
            case GameEngine::GameObjectEvent::TransformChanged:
                break;
        }

        auto gameEditor = dynamic_cast<GameEditor*>(gameObject()->game());
        gameEditor->setDirty(true);
    }
}
