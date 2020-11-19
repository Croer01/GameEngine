//
// Created by adria on 18/09/2020.
//

#include <iostream>
#include <game-engine/components/GeometryComponent.hpp>
#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include "GameEditor.hpp"

using namespace GameEngine;

GameEditor::GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment) : Game(environment)
{
    selectedObject_ = nullptr;
    dirty_ = false;
    environment->registerComponent("GameEditorComponent", new GameEngine::ComponentTBuilder<GameEditorComponent>());
}

GameEngine::geGameObjectRef GameEditor::createFromPrototype(const std::string &prototype)
{
    const GameEngine::geGameObjectRef &obj = Game::createFromPrototype(prototype);
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

    if(selectedObject_)
    {
        moveSelectedObject();
        targetObject_->position(selectedObject_->gameObject()->position());
    }

    if(targetObject_)
        targetObject_->Update(elapsedTime);


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
    createTargetSelectedObject();
    drag_ = false;
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
        Vector2DData position = prototypeRef->getPosition();
        object->position(GameEngine::Vec2D(position.xy[0],position.xy[1]));
        Vector2DData scale = prototypeRef->getScale();
        object->scale(GameEngine::Vec2D(scale.xy[0],scale.xy[1]));
        object->rotation(prototypeRef->getRotation());

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

void GameEditor::setSelected(GameEditorComponent *selectedObject)
{
    selectedObject_ = selectedObject;
    targetObject_->active(true);
    // default size will have the selection rectangle if the object doesn't have a graphic component
    Vec2D size = Vec2D(10.f, 10.f);
    std::string anchor = "TOP_LEFT";

    if(auto sprite = selectedObject_->gameObject()->getComponent<SpriteComponent>().lock()) {
        size = GameEngine::Vec2D(sprite->getWidth(), sprite->getHeight());
        anchor = sprite->anchor();
    }
    else if(auto spriteAnimated = selectedObject_->gameObject()->getComponent<SpriteAnimatedComponent>().lock()){
        size = GameEngine::Vec2D(spriteAnimated->getWidth(), spriteAnimated->getHeight());
        anchor = spriteAnimated->anchor();
    }
    else if(auto geometry = selectedObject_->gameObject()->getComponent<GeometryComponent>().lock()){
        size = GameEngine::Vec2D(geometry->getWidth(), geometry->getHeight());
        anchor = geometry->anchor();
    }

    size *= selectedObject_->gameObject()->scale();

    std::vector<Vec2D> path;
    path.emplace_back(0.f,0.f);
    path.emplace_back(size.x,0.f);
    path.emplace_back(size.x,size.y);
    path.emplace_back(0.f,size.y);
    targetObject_->getComponent<GeometryComponent>().lock()->path(path);
    targetObject_->getComponent<GeometryComponent>().lock()->anchor(anchor);
    targetObject_->position(selectedObject_->gameObject()->position());
}

void GameEditor::createTargetSelectedObject()
{
    auto object = Game::createObject("TargetSelected");
    targetObject_ = std::dynamic_pointer_cast<GameEngine::Internal::GameObject>(object);

    auto component =  objectManager()->createComponent("GeometryComponent");
    auto geomComponent =  std::dynamic_pointer_cast<GameEngine::GeometryComponent>(component);
    std::vector<Vec2D> path;
    path.emplace_back(0.f,0.f);
    path.emplace_back(100.f,0.f);
    path.emplace_back(100.f,100.f);
    path.emplace_back(0.f,100.f);
    geomComponent->path(path);
    geomComponent->fill(false);
    geomComponent->anchor("MIDDLE_CENTER");
    geomComponent->color(geColor(0.f,1.f,0.f));

    targetObject_->addComponent(component);

    targetObject_->preInit();
    targetObject_->Init();
}

void GameEditor::moveSelectedObject()
{
    geGameObject *gameObject =  selectedObject_->gameObject();
    const GameEngine::Vec2D &mousePosition = input()->getMousePosition();
    GameEngine::Vec2D mouseDistance = gameObject->position() - mousePosition;

    if(drag_)
    {
        gameObject->position(gameObject->position() + mousePosition - lastMousePos_);
    }
    else if(std::abs(mouseDistance.sqrMagnitude()) <= 50 * 50)
    {
        if(input()->isMouseButtonDown(GameEngine::MouseButton::LEFT))
        {
            drag_ = true;
        }
    }

    if(input()->isMouseButtonUp(GameEngine::MouseButton::LEFT))
    {
        drag_ = false;
    }

    lastMousePos_ = mousePosition;
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

    if(std::abs(mouseDistance.sqrMagnitude()) <= 50 * 50)
    {
        if(input->isMouseButtonDown(GameEngine::MouseButton::LEFT))
        {
            auto gameEditor = dynamic_cast<GameEditor*>(gameObject()->game());
            gameEditor->setSelected(this);
        }
    }
}

void GameEditorComponent::init()
{
    gameObject()->registerObserver(this);
}

void GameEditorComponent::linkObject(PrototypeReference *data)
{
    if(data_)
        data_->unregisterObserver(this);
    data_ = data;
    data_->registerObserver(this);
}

void GameEditorComponent::onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target,
                                  const GameEngine::GameObjectEvent &event, void *args)
{
    if(data_)
    {
        switch(event)
        {
            case GameEngine::GameObjectEvent::PositionChanged:
            {
                Vector2DData position;
                position.xy[0] = gameObject()->position().x;
                position.xy[1] = gameObject()->position().y;
                data_->setPosition(position);
            }
                break;
            case GameEngine::GameObjectEvent::RotationChanged:
                data_->setRotation(gameObject()->rotation());
                break;
            case GameEngine::GameObjectEvent::ScaleChanged:
            {
                Vector2DData scale;
                scale.xy[0] = gameObject()->scale().x;
                scale.xy[1] = gameObject()->scale().y;
                data_->setScale(scale);
            }
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

GameEditorComponent::~GameEditorComponent()
{
    if(data_)
        data_->unregisterObserver(this);
}

void GameEditorComponent::onEvent(const GameEngine::Subject<PrototypeReferenceEvent> &target,
                                  const PrototypeReferenceEvent &event, void *args)
{
    assert(data_);
    switch(event)
    {
        case PrototypeReferenceEvent::PositionChanged:
        {
            const Vector2DData &dataPos = data_->getPosition();
            GameEngine::Vec2D position(dataPos.xy[0], dataPos.xy[1]);
            gameObject()->position(position);
        }
            break;
        case PrototypeReferenceEvent::RotationChanged:
            gameObject()->rotation(data_->getRotation());
            break;
        case PrototypeReferenceEvent::ScaleChanged:
        {
            const Vector2DData &dataScale = data_->getScale();
            GameEngine::Vec2D scale(dataScale.xy[0], dataScale.xy[1]);
            gameObject()->scale(scale);
        }
            break;
    }
}
