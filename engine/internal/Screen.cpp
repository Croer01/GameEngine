//
// Created by adria on 28/10/2018.
//

#include <cmath>
#include <yaml-cpp/yaml.h>
#include <GL/glew.h>
#include <iostream>
#include <functional>
#include <game-engine/internal/Screen.hpp>
#include <game-engine/internal/utils.hpp>

#define DEFAULT_WINDOW_SIZE 512

namespace GameEngine {
namespace Internal {
    int resizingEventWatcher(void* data, SDL_Event* event) {
        Screen *target = static_cast<Screen*>(data);

        if (event->type == SDL_WINDOWEVENT &&
            event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {

            target->windowWidth(event->window.data1);
            target->windowHeight(event->window.data2);
        }
        return 0;
    }

    Screen::Screen(const std::string &filePath, bool embedded) {
        YAML::Node screenConfig;
        try {
            screenConfig = YAML::LoadFile(filePath);
        } catch (const YAML::BadFile &e){
            std::cout << "the file \"" << filePath << "\" not found. Using default configuration." << std::endl;
        }


        title_ = screenConfig["title"].as<std::string>("Game");

        if(screenConfig["windowSize"]){
            YAML::Node windowSize = screenConfig["windowSize"];
            deviceWidth_ = windowSize[0].as<int>();
            deviceHeight_ = windowSize[1].as<int>();
        } else{
            deviceWidth_ = DEFAULT_WINDOW_SIZE;
            deviceHeight_ = DEFAULT_WINDOW_SIZE;
        }

        if(screenConfig["virtualSize"]) {
            YAML::Node virtualSize = screenConfig["virtualSize"];
            virtualWidth_ = virtualSize[0].as<int>();
            virtualHeight_ = virtualSize[1].as<int>();
        } else {
            virtualWidth_ = DEFAULT_WINDOW_SIZE;
            virtualHeight_ = DEFAULT_WINDOW_SIZE;
        }
        recalculateWindow();

        if(screenConfig["backgroundColor"]){
            YAML::Node backgroundColor = screenConfig["backgroundColor"];
            background_ = geColor(backgroundColor[0].as<float>(), backgroundColor[1].as<float>(),
                                    backgroundColor[2].as<float>());
        } else {
            // blue corn color by default
            background_ = geColor(0.3922f, 0.5843f,0.9294f);
        }
        pixelPerfect_ = screenConfig["pixelPerfect"].as<bool>(false);
        allowResize_ = screenConfig["resizable"].as<bool>(false);

        if(!embedded)
            initSDLWindow();

        //configure Gl context from the SDL window
        initGlAttributes();
    }

    std::string Screen::title() const {
        return title_.get();
    }

    void Screen::title(const std::string &value) {
        title_ = value;
    }

    int Screen::windowWidth() const {
        return deviceWidth_;
    }

    void Screen::windowWidth(int value) {
        deviceWidth_ = value;
        recalculateWindow();
    }
    int Screen::windowHeight() const {
        return deviceHeight_;
    }

    void Screen::windowHeight(int value) {
        deviceHeight_ = value;
        recalculateWindow();
    }

    geColor Screen::backgroundColor() const {
        return background_;
    }

    void Screen::backgroundColor(geColor value) {
        background_ = value;
        glClearColor(background_.r, background_.g, background_.b, 1.f);
    }

    int Screen::virtualWidth() const {
        return virtualWidth_;
    }
    void Screen::virtualWidth(int value) {
        virtualWidth_ = value;
        recalculateWindow();
    }

    int Screen::virtualHeight() const {
        return virtualHeight_;
    }

    void Screen::virtualHeight(int value) {
        virtualHeight_ = value;
        recalculateWindow();
    }

    bool Screen::pixelPerfect() const {
        return pixelPerfect_;
    }

    void Screen::pixelPerfect(bool value) {
        pixelPerfect_ = value;
    }

    int Screen::calculatedX() const {
        return calculatedX_;
    }

    int Screen::calculatedY() const {
        return calculatedY_;
    }

    int Screen::calculatedWidth() const {
        return calculatedWidth_;
    }

    int Screen::calculatedHeight() const {
        return calculatedHeight_;
    }

    void Screen::recalculateWindow() {
        //Calculate the real size of the virtual screen when it will render
        float targetAspectRatio = (float) virtualWidth_ / virtualHeight_;

        // figure out the largest area that fits in this resolution at the desired aspect ratio
        calculatedWidth_ = deviceWidth_;
        calculatedHeight_ = std::lround(calculatedWidth_ / targetAspectRatio + 0.5f);

        if (calculatedHeight_ > deviceHeight_) {
            //It doesn't fit our height, we must switch to pillarbox then
            calculatedHeight_ = deviceHeight_;
            calculatedWidth_ = std::lround(calculatedHeight_ * targetAspectRatio + 0.5f);
        }

        // set up the new viewport centered in the backbuffer
        calculatedX_ = (deviceWidth_ / 2) - (calculatedWidth_ / 2);
        calculatedY_ = (deviceHeight_ / 2) - (calculatedHeight_ / 2);
    }

    void Screen::initGlAttributes()
    {
        glViewport(calculatedX_, calculatedY_, calculatedWidth_, calculatedHeight_);

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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

#if DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        if(mainWindow_)
        {
            mainContext_ = SDL_GL_CreateContext(mainWindow_.get());
            makeCurrentContext();
        }

        // This makes our buffer swap synchronized with the monitor's vertical refresh
        // Enable vsync
        SDL_GL_SetSwapInterval(1);

        // Init GLEW
        // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know <- (original comment)
#ifndef __APPLE__
        glewExperimental = GL_TRUE;
        glewInit();
#endif

        //Initialize clear color
        glClearColor(background_.r, background_.g, background_.b, 1.f);

        CheckGlError();
        CheckSDLError();
    }

    void Screen::initSDLWindow() {
        uint32_t flags = SDL_WINDOW_OPENGL;
        if(allowResize_)
            flags |= SDL_WINDOW_RESIZABLE;
        mainWindow_.reset(SDL_CreateWindow(title_.get().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                           deviceWidth_, deviceHeight_, flags),
                          [](SDL_Window *wind) { SDL_DestroyWindow(wind); });
        SDL_AddEventWatch(resizingEventWatcher, this);
        CheckSDLError();
    }

    void Screen::makeCurrentContext() {
        if(mainWindow_)
            SDL_GL_MakeCurrent(mainWindow_.get(), mainContext_);
    }

    void Screen::swapWindow() {
        if(mainWindow_)
            SDL_GL_SwapWindow(mainWindow_.get());
    }

    Screen::~Screen() {
        SDL_GL_DeleteContext(mainContext_);
        mainWindow_.reset();
    }

    bool Screen::resizable() const {
        return allowResize_;
    }

    void Screen::resizable(bool value) {
        //TODO: check this. Maybe in embedded games this doesn't work correctly
        allowResize_ = value;
        SDL_SetWindowResizable(mainWindow_.get(), allowResize_? SDL_TRUE : SDL_FALSE);
    }

    SDL_Window *Screen::sdlWindow() const {
        return mainWindow_.get();
    }

void Screen::update()
{
    if(mainWindow_ && title_.update())
        SDL_SetWindowTitle(mainWindow_.get(), title_.get().c_str());
}

void Screen::setWindowRelativePosition(int x, int y)
{
    windowRelativePos_ = Vec2D(x, y);
}

Vec2D Screen::getWindowRelativePosition()
{
    Vec2D screenPos(calculatedX_, calculatedY_);
    return windowRelativePos_;
}

Vec2D Screen::transformWindowToScreen(const Vec2D &position)
{
    Vec2D windowRelativePosition = getWindowRelativePosition();
    float xScale = (float)virtualWidth_ / deviceWidth_;
    float yScale = (float)virtualHeight_ / deviceHeight_;

    Vec2D screenPosition;
    screenPosition.x = (position.x - windowRelativePosition.x) * xScale;
    screenPosition.y = (position.y - windowRelativePosition.y) * yScale;
    return screenPosition;
}

}
}