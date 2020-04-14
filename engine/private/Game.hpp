//
// Created by adria on 22/09/2018.
//

#ifndef GAMEENGINE_GAME_HPP
#define GAMEENGINE_GAME_HPP


#include <game-engine/geGame.hpp>
#include <memory>
#include "Screen.hpp"
#include "ObjectManager.hpp"
#include "graphics/GraphicsEngine.hpp"
#include "audio/AudioEngine.hpp"
#include "physics/PhysicsEngine.hpp"
#include "SceneManager.hpp"
#include "Environment.hpp"
#include "graphics/font/FontManager.hpp"
#include <string>
#include <game-engine/InputManager.hpp>
#include <mutex>

namespace GameEngine {
namespace Internal {

class RendererLock : public geRendererLock
{
    std::mutex &mutex_;
    bool unlocked_;
public:
    RendererLock(const RendererLock&) = delete;
    explicit RendererLock(std::mutex &mutex);

    virtual void unlock();
    virtual ~RendererLock();
};

class Game : public geGame {
        bool running_;

        std::shared_ptr<Environment> environment_;
        std::unique_ptr<Screen> screen_;
        std::unique_ptr<GraphicsEngine> graphicsEngine_;
        std::unique_ptr<AudioEngine> audioEngine_;
        std::unique_ptr<PhysicsEngine> physicsEngine_;
        std::unique_ptr<InputManager> inputManager_;
        std::unique_ptr<FontManager> fontManager_;
        unsigned int lastTime_;
        std::mutex renderMutex_;

        void initPhysics(const std::string &configFilePath);

public:
        Game(const std::shared_ptr<Environment> &environment);
        virtual ~Game();

        const geGame &context() const;

        // geGame implementation
        virtual void update();
        virtual void render();
        virtual unsigned int getRenderer() const;
        virtual geRendererLock getRendererLock();
        virtual bool isRunning() const;
        void shutdown() override;
        geGameObjectRef createObject(const std::string &name) override;
        geGameObjectRef createFromPrototype(const std::string &prototype) override;
        geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName) override;
        void changeScene(const std::string &name) override;
        std::weak_ptr<geCamera> cameraOfCurrentScene() const override;
        geScreen *screen() const override;
        geAudio *audio() const override;
        InputManager *input() const override;

        // GameEngine internal API
        GraphicsEngine *graphicsEngine() const;
        PhysicsEngine *physicsEngine() const;
        AudioEngine *audioEngine() const;
        FontManager *fontManager() const;
        ObjectManager *objectManager() const;
    };
}
}

#endif //GAMEENGINE_GAME_HPP
