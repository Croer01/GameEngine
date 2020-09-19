//
// Created by adria on 18/09/2020.
//

#ifndef DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
#define DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP

#include "../engine/private/Game.hpp"

class GameEditorComponent : public GameEngine::geComponentInstantiable<GameEditorComponent>
{
    bool drag_;
    GameEngine::Vec2D lastMousePos_;
public:
    virtual GameEngine::PropertySetBase *getProperties() const;
    virtual void init();
    virtual void Update(float elapsedTime);
};

class GameEditor : public GameEngine::Internal::Game
{
    std::vector<std::weak_ptr<GameEditorComponent>> components_;
public:
    explicit GameEditor(const std::shared_ptr<GameEngine::Internal::Environment> &environment);
    GameEngine::geGameObjectRef createObject(const std::string &name) override;
    GameEngine::geGameObjectRef createFromPrototype(const std::string &prototype) override;
    virtual void changeScene(const std::string &name);
    void update() override;
    virtual ~GameEditor(){};
};


#endif //DANGEROUSROBOTSINTHEDESERT_GAMEEDITOR_HPP
