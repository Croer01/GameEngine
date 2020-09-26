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
#include "TimeManager.hpp"
#include <string>
#include <game-engine/InputManager.hpp>
#include <mutex>

namespace GameEngine {
namespace Internal {

class Game : public geGame {
    bool running_;
    std::mutex renderMutex_;
    Vec2D rendererPosition_;
protected:
    bool initialized_;
        std::shared_ptr<Environment> environment_;
        std::unique_ptr<Screen> screen_;
        std::unique_ptr<GraphicsEngine> graphicsEngine_;
        std::unique_ptr<AudioEngine> audioEngine_;
        std::unique_ptr<PhysicsEngine> physicsEngine_;
        std::unique_ptr<InputManager> inputManager_;
        std::unique_ptr<FontManager> fontManager_;
        std::unique_ptr<TimeManager> timeManager_;

        void initPhysics(const std::string &configFilePath);
public:
        explicit Game(const std::shared_ptr<Environment> &environment);
        virtual ~Game();

        const geGame &context() const;

        // geGame implementation
        virtual void init();
        virtual void update();
        virtual void render();
        virtual unsigned int getRenderer() const;
        virtual std::mutex &getRendererMutex();
        virtual bool isRunning() const;
        void shutdown() override;
        virtual geGameObjectRef createObject(const std::string &name);
        virtual geGameObjectRef createFromPrototype(const std::string &prototype);
        geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName) override;
        void changeScene(const std::string &name) override;
        std::weak_ptr<geCamera> cameraOfCurrentScene() const override;
        virtual geDataRef saveCurrentSceneState() const;
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
