//
// Created by adria on 22/09/2018.
//

#include <game-engine/Game.hpp>
#include <iostream>
#include <game-engine/internal/utils.hpp>
#include <SDL.h>
#include <memory>

using namespace GameEngine::Internal;

namespace GameEngine {
    GameRef Game::createInstance(const geEnvironmentRef &env)
    {
        auto envInternal = std::dynamic_pointer_cast<Internal::Environment>(env);
        auto game = std::shared_ptr<Game>(new Game(envInternal));
        return game;
    }

    Game::Game(const std::shared_ptr<Environment> &environment)
    {
        initialized_ = false;
        std::unique_lock<std::mutex> lock(renderMutex_);
        running_ = true;
        environment_ = environment;

        // Initialize SDL's Video subsystem
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            CheckSDLError();
        }

        if(environment_->getMakeCurrentContextCallback())
        {
            environment_->getMakeCurrentContextCallback()();
            CheckSDLError();
            CheckGlError();
        }
        bool embeddedGraphics = (bool)environment_->getMakeCurrentContextCallback();
        screen_ = std::make_unique<Screen>(environment_->configurationPath() + "/screen.yaml", embeddedGraphics);

        graphicsEngine_ = std::make_unique<GraphicsEngine>(screen_.get(), embeddedGraphics);

        audioEngine_ = std::make_unique<AudioEngine>();
        audioEngine_->init();
        fontManager_ = std::make_unique<FontManager>(environment_->getAbsoluteDataPath());
        inputManager_ = std::make_unique<InputManager>(screen_.get());

        timeManager_ = std::make_unique<TimeManager>(this);
        initPhysics(environment_->configurationPath() + "/physics.yaml");
    }

    Game::~Game()
    {
        std::unique_lock<std::mutex> lock(renderMutex_);
        timeManager_.reset();
        environment_->sceneManager()->clear();
        // destroy all the engines and APIs to ensure the components won't try to use them
        // in a inconsistence state
        graphicsEngine_.reset(nullptr);
        audioEngine_.reset(nullptr);
        physicsEngine_.reset(nullptr);
        inputManager_.reset(nullptr);
        fontManager_.reset(nullptr);

        //TODO: should this call in shutdown instead of here?
        if(!environment_->getMakeCurrentContextCallback())
            SDL_Quit();

        //TODO: create a way to bind game into components
        environment_->sceneManager()->unbindGame();
    }

void Game::initPhysics(const std::string &configFilePath) {
        physicsEngine_ = std::make_unique<PhysicsEngine>();
        //TODO: improve how to calculate the elapsed time to use in physics
#ifdef DEBUG
        physicsEngine_->init(timeManager_->getPhysicsElapsedTime(), screen_.get());
#else
        physicsEngine_->init(timeManager_->getPhysicsElapsedTime());
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

    void Game::update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            inputManager_->addEvent(event);
        }

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
        if (inputManager_->isKeyDown(KeyCode::KEY_F5)) {
            environment_->sceneManager()->reloadScene();
        }

        // This is a dirty solution to avoid big elapsed times when debugging.
        // A common situation is when the execution is stopped a long time by a breakpoint.
        if (elapsedTime >= 5)
            elapsedTime = 1.f / 60.f;
#endif
        environment_->sceneManager()->update(elapsedTime);
        physicsEngine_->update(elapsedTime);
    }

    void Game::render()
    {
        std::unique_lock<std::mutex> lock(renderMutex_);
        const MakeCurrentContextCallback &makeContext = environment_->getMakeCurrentContextCallback();

        if(makeContext)
        {
            makeContext();
            CheckSDLError();
            CheckGlError();
        }

        auto cam = dynamic_cast<Camera*>(environment_->sceneManager()->getCameraOfCurrentScene());
        graphicsEngine_->draw(cam);
#ifdef DEBUG
        if(graphicsEngine_->getFbo() != nullptr)
        {
            graphicsEngine_->getFbo()->bind();
            physicsEngine_->drawDebug(cam);
            graphicsEngine_->getFbo()->unBind();
        }
        else
            physicsEngine_->drawDebug(cam);
#endif

        if (makeContext)
        {
            // before change context we ensure all is drawn in the FBO
            glFinish();
        }
        else
        {
            screen_->swapWindow();
        }
    }

    void Game::shutdown() {
        running_ = false;
    }

    const Game &Game::context() const {
        return (const Game &) *this;
    }

    geGameObjectRef Game::createObject(const std::string &name)
    {
        const std::shared_ptr<Internal::GameObject> &object = std::make_shared<Internal::GameObject>("");
        object->name(name);
        object->game(this);
        if(environment_->sceneManager()->isSceneLoaded())
            environment_->sceneManager()->addObjectIntoCurrentScene(object);

        return object;
    }

    geGameObjectRef Game::createFromPrototype(const std::string &prototype)
    {
        const std::shared_ptr<Internal::GameObject> &object = environment_->objectManager()->createGameObject(prototype, this);
        if(environment_->sceneManager()->isSceneLoaded())
            environment_->sceneManager()->addObjectIntoCurrentScene(object);

        return object;
    }

    geGameObjectRef Game::findObjectByNameInCurrentScene(const std::string &gameObjectName)
    {
        const std::shared_ptr<GameObject> &object = environment_->sceneManager()->findObjectByName(gameObjectName);
        return std::dynamic_pointer_cast<geGameObject>(object);
    }

    geCamera* Game::cameraOfCurrentScene() const
    {
        return environment_->sceneManager()->getCameraOfCurrentScene();
    }

    geScreen * Game::screen() const
    {
        return screen_.get();
    }

    geAudio * Game::audio() const
    {
        return audioEngine_.get();
    }

GraphicsEngine *Game::graphicsEngine() const
{
    return graphicsEngine_.get();
}

PhysicsEngine *Game::physicsEngine() const
{
    return physicsEngine_.get();
}

AudioEngine *Game::audioEngine() const
{
    return audioEngine_.get();
}

InputManager *Game::input() const
{
    return inputManager_.get();
}

FontManager *Game::fontManager() const
{
    return fontManager_.get();
}

ObjectManager *Game::objectManager() const
{
    return environment_->objectManager();
}

SceneManager *Game::sceneManager() const
{
    return environment_->sceneManager();
}

bool Game::isRunning() const
{
    return running_;
}

unsigned int Game::getRenderer() const
{
    return graphicsEngine_->getFbo()->getTextId();
}

std::mutex & Game::getRendererMutex()
{
    return renderMutex_;
}

void Game::init()
{
    initialized_ = true;

    environment_->sceneManager()->bindGame(this);
    if(!environment_->firstScene().empty())
    {
        environment_->sceneManager()->changeScene(environment_->firstScene());
        environment_->sceneManager()->changeSceneInSafeMode();
    }
}

geDataRef Game::saveCurrentSceneState() const
{
    return environment_->sceneManager()->saveCurrentSceneState();
}

GlobalData *Game::getGlobalData() const
{
    return environment_->getGlobalData();
}

}
