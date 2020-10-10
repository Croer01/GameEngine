//
// Created by adria on 18/09/2020.
//

#ifndef DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
#define DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP

#include "../engine/private/Game.hpp"
#include "ViewModels.hpp"

class GameEditorComponent : public GameEngine::geComponentInstantiable<GameEditorComponent>, public GameEngine::Observer<GameEngine::GameObjectEvent>
{
    bool drag_;
    GameEngine::Vec2D lastMousePos_;
    PrototypeReferenceRef data_;
public:
    virtual GameEngine::PropertySetBase *getProperties() const;
    virtual void init();
    virtual void Update(float elapsedTime);
    void linkObject(const PrototypeReferenceRef &data);
    virtual void
    onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event,
            void *args);
};

class GameEditor : public GameEngine::Internal::Game
{
    std::vector<std::weak_ptr<GameEditorComponent>> components_;
    SceneDataRef sceneData_;
    void addEditorComponent(const GameEngine::geGameObjectRef &object);
    void linkSceneDataWithCurrentScene();
public:
    explicit GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment);
    GameEngine::geGameObjectRef createObject(const std::string &name) override;
    GameEngine::geGameObjectRef createFromPrototype(const std::string &prototype) override;
    virtual void changeScene(const std::string &name);
    virtual void init();
    void update() override;
    void linkSceneFromEditor(const SceneDataRef &scene);

    virtual ~GameEditor(){};
};


#endif //DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
