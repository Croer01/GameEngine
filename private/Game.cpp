//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include "Game.hpp"
#include "ObjectManager.hpp"
#include "utils.hpp"
#include <game-engine/components/SpriteComponent.hpp>
#include "graphics/GraphicsEngine.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include <game-engine/InputManager.hpp>
#include <game-engine/components/ColliderComponent.hpp>
#include "physics/PhysicsEngine.hpp"
#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include "audio/AudioEngine.hpp"
#include "game-engine/components/UIButtonComponent.hpp"
#include "game-engine/components/ui/UITextComponent.hpp"
#include <game-engine/components/AudioComponent.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <game-engine/components/GeometryComponent.hpp>

namespace GameEngine {
namespace Internal {

    void Game::onCreateInstance() {
        //Register engine default components
        ObjectManager::GetInstance().registerComponentBuilder("SpriteComponent",
                                                              new ComponentTBuilder<SpriteComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("GeometryComponent",
                                                              new ComponentTBuilder<GeometryComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("SpriteAnimatedComponent",
                                                              new ComponentTBuilder<SpriteAnimatedComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("ColliderComponent",
                                                              new ComponentTBuilder<ColliderComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("TextComponent", new ComponentTBuilder<TextComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("AudioComponent",
                                                              new ComponentTBuilder<AudioComponent>());
        //GUI Components
        ObjectManager::GetInstance().registerComponentBuilder("UIButtonComponent", new ComponentTBuilder<UIButtonComponent>());
        ObjectManager::GetInstance().registerComponentBuilder("UITextComponent", new ComponentTBuilder<UITextComponent>());
    }

    void Game::init(const std::string &configRoot) {
        running_ = true;
        // Initialize SDL's Video subsystem
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            CheckSDLError();
        }
        screen_.reset( new Screen(configRoot + "/screen.yaml"));
        GraphicsEngine::GetInstance().init(*screen_);

        initPhysics(configRoot + "/physics.yaml");

        AudioEngine::GetInstance().init();
    }

    void Game::initPhysics(const std::string &configFilePath) {
        PhysicsEngine::GetInstance().init(1.f / 60.f);

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
            PhysicsEngine::GetInstance().createCategories(categories);
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

            PhysicsEngine::GetInstance().createMasks(masks);
        }
    }

    void Game::loop() {
        unsigned int lastTime = 0, currentTime;

        while (running_) {
            InputManager::GetInstance().update();
            if (InputManager::GetInstance().isQuitDown()) {
                shutdown();
                continue;
            }

#ifdef DEBUG
            if (InputManager::GetInstance().isKeyDown(KeyCode::KEY_F5)) {
                SceneManager::GetInstance().reloadScene();
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
            SceneManager::GetInstance().update(elapsedTime);
            PhysicsEngine::GetInstance().update(elapsedTime);

            glClear(GL_COLOR_BUFFER_BIT);
            glViewport(screen_->calculatedX(), screen_->calculatedY(), screen_->calculatedWidth(),
                       screen_->calculatedHeight());
            const std::shared_ptr<Camera> &cam = SceneManager::GetInstance().getCameraOfCurrentScene();
            GraphicsEngine::GetInstance().draw(cam);
#ifdef DEBUG
            PhysicsEngine::GetInstance().drawDebug(cam);
#endif
            screen_->swapWindow();
            lastTime = currentTime;

            SceneManager::GetInstance().changeSceneInSafeMode();
        }

        // shutdown or clear all Game Systems
        SceneManager::GetInstance().clear();
        ObjectManager::GetInstance().clear();
        // Restore default Components' Builder
        onCreateInstance();

        //TODO: Do physics and graphics engines shutdown?
        screen_.reset();

        // Shutdown SDL 2
        SDL_Quit();
    }


    void Game::shutdown() {
        running_ = false;
        context_ = nullptr;
    }

    void Game::context(geGame *context) {
        context_ = context;
    }

    geGame &Game::context() const {
        return *context_;
    }

    Screen &Game::screen() const {
        return *screen_;
    }
}
}