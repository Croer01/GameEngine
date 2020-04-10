//
// Created by adria on 22/09/2018.
//

#include "Game.hpp"
#include <iostream>
#include "utils.hpp"
#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <memory>

namespace GameEngine {
namespace Internal {

    Game::Game(const std::shared_ptr<Environment> &environment)
    {
        running_ = false;
        environment_ = environment;
    }

    Game::~Game()
    {
        // TODO: implement destructor?
    }

    void Game::init() {
        if(!running_)
        {
            running_ = true;
            // Initialize SDL's Video subsystem
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                CheckSDLError();
            }
            screen_ = std::make_shared<Screen>(environment_->configurationPath() + "/screen.yaml");
            graphicsEngine_ = std::make_shared<GraphicsEngine>();
            graphicsEngine_->init(*screen_);

            initPhysics(environment_->configurationPath() + "/physics.yaml");

            audioEngine_ = std::make_shared<AudioEngine>();
            audioEngine_->init();
            fontManager_ = std::make_shared<FontManager>();
            inputManager_ = std::make_shared<InputManager>();

            environment_->sceneManager()->bindGame(shared_from_this());
            if(!environment_->firstScene().empty())
            {
                changeScene(environment_->firstScene());
                environment_->sceneManager()->changeSceneInSafeMode();
            }
        }
    }

    void Game::initPhysics(const std::string &configFilePath) {
        physicsEngine_ = std::make_shared<PhysicsEngine>();
#ifdef DEBUG
        physicsEngine_->init(1.f / 60.f, screen_.get());
#else
        physicsEngine_->init(1.f / 60.f);
#endif
        YAML::Node physicsConfig;
        try {
            physicsConfig = YAML::LoadFile(configFilePath);
        } catch (const YAML::BadFile &e){
            std::cout << "the file \"" << configFilePath << "\" not found. Using default configuration." << std::endl;
        }

        //TODO: implement category/mask configuration as a object
        if (physicsConfig["categories"]) {
            YAML::Node categoriesNode = physicsConfig["categories"];

            std::vector<std::string> categories;
            categories.reserve(categoriesNode.size());
            for (int i = 0; i < categoriesNode.size(); ++i) {
                categories.push_back(categoriesNode[i].as<std::string>());
            }
            physicsEngine_->createCategories(categories);
        }
        if (physicsConfig["masks"]) {
            std::unordered_map<std::string, std::vector<std::string>> masks;

            YAML::Node masksNode = physicsConfig["masks"];
            for (YAML::const_iterator it = masksNode.begin(); it != masksNode.end(); ++it) {
                std::string category = it->first.as<std::string>();
                std::vector<std::string> mask;
                for (int i = 0; i < it->second.size(); ++i) {
                    mask.push_back(it->second[i].as<std::string>());
                }
                masks.insert(std::pair<std::string, std::vector<std::string>>(category, mask));
            }

            physicsEngine_->createMasks(masks);
        }
    }

    void Game::innerLoop() {
        unsigned int lastTime = 0, currentTime;

        while (running_) {
            inputManager_->update();
            if (inputManager_->isQuitDown()) {
                shutdown();
                continue;
            }

#ifdef DEBUG
            if (inputManager_->isKeyDown(KeyCode::KEY_F5)) {
                environment_->sceneManager()->reloadScene();
            }
#endif

            //calculate elapsed time
            currentTime = SDL_GetTicks();
            float elapsedTime = (currentTime - lastTime) / 1000.f;
#ifdef DEBUG
            //this is a dirty solution to avoid big elapsed times when debugging.
            // Common situation is the execution stops in breakpoint to a long time.
            if (elapsedTime >= 5)
                elapsedTime = 1.f / 60.f;
#endif
            environment_->sceneManager()->update(elapsedTime);
            physicsEngine_->update(elapsedTime);

            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(screen_->calculatedX(), screen_->calculatedY(), screen_->calculatedWidth(),
                       screen_->calculatedHeight());
            const std::shared_ptr<Camera> &cam = environment_->sceneManager()->getCameraOfCurrentScene();
            graphicsEngine_->draw(cam);
#ifdef DEBUG
            physicsEngine_->drawDebug(cam);
#endif
            screen_->swapWindow();
            lastTime = currentTime;

            environment_->sceneManager()->changeSceneInSafeMode();
        }

        //TODO: should this call in shutdown instead of here?
        SDL_Quit();
    }


    void Game::shutdown() {
        running_ = false;
    }

    const geGame &Game::context() const {
        return (const geGame &) *this;
    }

    int Game::loop() {
        try {
            init();

            innerLoop();
            //TODO: create a way to bind game into components
            environment_->sceneManager()->unbindGame();
            return 0;
        }
        catch (const std::exception &e){
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    geGameObjectRef Game::createObject(const std::string &name)
    {
        const std::shared_ptr<Internal::GameObject> &object = std::make_shared<Internal::GameObject>("");
        object->name(name);
        if(environment_->sceneManager()->isSceneLoaded())
            environment_->sceneManager()->addObjectIntoCurrentScene(object);

        return object;
    }

    geGameObjectRef Game::createFromPrototype(const std::string &prototype)
    {
        const std::shared_ptr<Internal::GameObject> &object = environment_->objectManager()->createGameObject(prototype, shared_from_this());
        if(environment_->sceneManager()->isSceneLoaded())
            environment_->sceneManager()->addObjectIntoCurrentScene(object);

        return object;
    }

    geGameObjectRef Game::findObjectByNameInCurrentScene(const std::string &gameObjectName)
    {
        const std::shared_ptr<GameObject> &object = environment_->sceneManager()->findObjectByName(gameObjectName);
        return std::dynamic_pointer_cast<geGameObject>(object);
    }

    void Game::changeScene(const std::string &name)
    {
        environment_->sceneManager()->changeScene(name);
    }

    std::weak_ptr<geCamera> Game::cameraOfCurrentScene() const
    {
        return environment_->sceneManager()->getCameraOfCurrentScene();
    }

    std::shared_ptr<geScreen> Game::screen() const
    {
        return screen_;
    }

    std::shared_ptr<geAudio> Game::audio() const
    {
        return audioEngine_;
    }

std::shared_ptr<GraphicsEngine> Game::graphicsEngine() const
{
    return graphicsEngine_;
}

std::shared_ptr<PhysicsEngine> Game::physicsEngine() const
{
    return physicsEngine_;
}

std::shared_ptr<AudioEngine> Game::audioEngine() const
{
    return audioEngine_;
}

std::shared_ptr<InputManager> Game::input() const
{
    return inputManager_;
}

std::shared_ptr<FontManager> Game::fontManager() const
{
    return fontManager_;
}

std::shared_ptr<ObjectManager> Game::objectManager() const
{
    return environment_->objectManager();
}

}
}