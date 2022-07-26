//
// Created by adria on 22/09/2018.
//

#ifndef GAMEENGINE_GAME_HPP
#define GAMEENGINE_GAME_HPP


#include <game-engine/api.hpp>
#include <memory>
#include <game-engine/internal/Screen.hpp>
#include <game-engine/internal/ObjectManager.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>
#include <game-engine/internal/audio/AudioEngine.hpp>
#include <game-engine/internal/physics/PhysicsEngine.hpp>
#include <game-engine/internal/SceneManager.hpp>
#include <game-engine/internal/Environment.hpp>
#include <game-engine/internal/graphics/font/FontManager.hpp>
#include <game-engine/internal/TimeManager.hpp>
#include <string>
#include <game-engine/InputManager.hpp>
#include <mutex>

namespace GameEngine {

class Game;
typedef std::shared_ptr<Game> GameRef;

class PUBLICAPI Game
{
protected:
    bool running_;
    std::mutex renderMutex_;

    bool initialized_;
    std::shared_ptr<Internal::Environment> environment_;
    std::unique_ptr<Internal::Screen> screen_;
    std::unique_ptr<Internal::GraphicsEngine> graphicsEngine_;
    std::unique_ptr<Internal::AudioEngine> audioEngine_;
    std::unique_ptr<Internal::PhysicsEngine> physicsEngine_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<Internal::FontManager> fontManager_;
    std::unique_ptr<Internal::TimeManager> timeManager_;

    void initPhysics(const std::string &configFilePath);
    explicit Game(const std::shared_ptr<Internal::Environment> &environment);
public:

    static GameRef createInstance(const std::shared_ptr<geEnvironment> &env);
    virtual ~Game();

    const Game &context() const;

    // geGame implementation
    virtual void init();
    virtual void update();
    virtual void render();
    unsigned int getRenderer() const;
    std::mutex &getRendererMutex();
    bool isRunning() const;
    void shutdown();
    virtual geGameObjectRef createObject(const std::string &name);
    virtual geGameObjectRef createFromPrototype(const std::string &prototype);
    geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName);
    geCamera *cameraOfCurrentScene() const;
    geDataRef saveCurrentSceneState() const;
    geScreen *screen() const;
    geAudio *audio() const;
    InputManager *input() const;

    // GameEngine internal API
    Internal::GraphicsEngine *graphicsEngine() const;
    Internal::PhysicsEngine *physicsEngine() const;
    Internal::AudioEngine *audioEngine() const;
    Internal::FontManager *fontManager() const;
    Internal::ObjectManager *objectManager() const;
    Internal::SceneManager *sceneManager() const;
};
}

#endif //GAMEENGINE_GAME_HPP
