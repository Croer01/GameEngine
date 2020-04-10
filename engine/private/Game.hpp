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

namespace GameEngine {
namespace Internal {
class Game : public geGame, public std::enable_shared_from_this<Game> {
        bool running_;

        std::shared_ptr<Screen> screen_;
        std::shared_ptr<GraphicsEngine> graphicsEngine_;
        std::shared_ptr<AudioEngine> audioEngine_;
        std::shared_ptr<PhysicsEngine> physicsEngine_;
        std::shared_ptr<InputManager> inputManager_;
        std::shared_ptr<FontManager> fontManager_;
        std::shared_ptr<Environment> environment_;

        void initPhysics(const std::string &configFilePath);
        void innerLoop();

    public:
        Game(const std::shared_ptr<Environment> &environment);
        virtual ~Game();

        void init(const std::string &configRoot);
        const geGame &context() const;

        // geGame implementation
        void init() override;
        int loop() override;
        void shutdown() override;
        geGameObjectRef createObject(const std::string &name) override;
        geGameObjectRef createFromPrototype(const std::string &prototype) override;
        geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName) override;
        void changeScene(const std::string &name) override;
        std::weak_ptr<geCamera> cameraOfCurrentScene() const override;
        std::shared_ptr<geScreen> screen() const override;
        std::shared_ptr<geAudio> audio() const override;
        std::shared_ptr<InputManager> input() const override;

        // GameEngine internal API
        std::shared_ptr<GraphicsEngine> graphicsEngine() const;
        std::shared_ptr<PhysicsEngine> physicsEngine() const;
        std::shared_ptr<AudioEngine> audioEngine() const;
        std::shared_ptr<FontManager> fontManager() const;
        std::shared_ptr<ObjectManager> objectManager() const;
    };
}
}

#endif //GAMEENGINE_GAME_HPP
