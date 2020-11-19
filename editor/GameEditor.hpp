//
// Created by adria on 18/09/2020.
//

#ifndef DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
#define DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP

#include "../engine/private/Game.hpp"
#include "ViewModels.hpp"

class GameEditorComponent : public GameEngine::geComponentInstantiable<GameEditorComponent>,
                            public GameEngine::Observer<GameEngine::GameObjectEvent>,
                            public GameEngine::Observer<PrototypeReferenceEvent>
{
    PrototypeReference *data_;
    GameEngine::Vec2D size_;
    GameEngine::Vec2D offsetFromRender_;
public:
    virtual ~GameEditorComponent();
    virtual GameEngine::PropertySetBase *getProperties() const;
    virtual void init();
    virtual void Update(float elapsedTime);
    void linkObject(PrototypeReference *data);
    bool isPointInside(GameEngine::Vec2D point);
    GameEngine::Vec2D getScaledSize() const;
    virtual void
    onEvent(const GameEngine::Subject<GameEngine::GameObjectEvent> &target, const GameEngine::GameObjectEvent &event,
            void *args);
    virtual void
    onEvent(const GameEngine::Subject<PrototypeReferenceEvent> &target, const PrototypeReferenceEvent &event,
            void *args);
};

class GameEditor : public GameEngine::Internal::Game, public GameEngine::Observer<SceneDataEvent>
{
    bool drag_;
    GameEngine::Vec2D lastMousePos_;
    GameEditorComponent *selectedObject_;
    std::shared_ptr<GameEngine::Internal::GameObject> targetObject_;
    std::vector<std::weak_ptr<GameEditorComponent>> components_;
    SceneDataRef sceneData_;
    bool dirty_;
    void addEditorComponent(const GameEngine::geGameObjectRef &object);
    void linkSceneDataWithCurrentScene();
    void createTargetSelectedObject();
    void moveSelectedObject();
public:
    virtual ~GameEditor();
    explicit GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment);
    GameEngine::geGameObjectRef createObject(const std::string &name) override;
    GameEngine::geGameObjectRef createFromPrototype(const std::string &prototype) override;
    virtual void changeScene(const std::string &name);
    virtual void init();
    void update() override;
    void linkSceneFromEditor(const SceneDataRef &scene);

    void setDirty(bool value);
    bool isDirty() const;
    virtual void onEvent(const GameEngine::Subject<SceneDataEvent> &target, const SceneDataEvent &event, void *args);
    void setSelected(GameEditorComponent *selectedObject);
};


#endif //DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
