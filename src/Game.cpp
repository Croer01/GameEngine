//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include "Game.hpp"
#include "ObjectManager.hpp"
#include "utils.hpp"
#include "components/SpriteComponent.hpp"
#include "graphics/GraphicsEngine.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>


void Game::init() {
    running_ = true;
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        CheckSDLError();
    }

    initSDLWindow();
    GraphicsEngine::GetInstance().init();

    //Register engine default components
    ObjectManager::GetInstance().registerComponentBuilder("SpriteComponent", new ComponentTBuilder<SpriteComponent>());
    RegisterComponents();
}

void Game::initSDLWindow() {
    //initialize SDL window
    //TODO: set this options from global configuration
    width_ = 512;
    height_ = 512;

    //TODO: set the title of the window from global configuration
    mainWindow_.reset(SDL_CreateWindow("GameMain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       width_, height_, SDL_WINDOW_OPENGL),
                      [](SDL_Window *wind) { SDL_DestroyWindow(wind); });
    CheckSDLError();

    //configure Gl context from the SDL window
    glViewport(0, 0, width_, height_);

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    mainContext_ = SDL_GL_CreateContext(mainWindow_.get());
    makeCurrentContext();

    // This makes our buffer swap synchronized with the monitor's vertical refresh
    // Enable vsync
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    //Initialize clear color
    //TODO: set the title of the window from global configuration
    glClearColor(0.39f, 0.58f, 0.93f, 1.f);

    CheckGlError();
    CheckSDLError();
}

void Game::makeCurrentContext() {
    SDL_GL_MakeCurrent(mainWindow_.get(), mainContext_);
}

void Game::loop() {
    std::shared_ptr<Scene> scene = SceneManager::GetInstance().getScene("Scene0");
    scene->init();

    unsigned int lastTime = 0, currentTime;

    while (running_){
        InputManager::GetInstance().update();
        if (InputManager::GetInstance().isQuitDown()) {
            shutdown();
            continue;
        }

        //calculate elapsed time
        currentTime = SDL_GetTicks();
        float elapsedTime = (currentTime - lastTime) / 1000.f;

        scene->update(elapsedTime);
        glClear(GL_COLOR_BUFFER_BIT);
        GraphicsEngine::GetInstance().draw();
        SDL_GL_SwapWindow(mainWindow_.get());
        lastTime = currentTime;
    }

    // Shutdown SDL 2
    SDL_Quit();
}


void Game::shutdown() {
    running_ = false;
}