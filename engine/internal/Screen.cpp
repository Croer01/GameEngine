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
#if DEBUG
#include <cstdio>
#endif

#define DEFAULT_WINDOW_SIZE 512

namespace GameEngine {
namespace Internal {
    int resizingEventWatcher(void* data, SDL_Event* event) {
        Screen *target = static_cast<Screen*>(data);

        if (event->type == SDL_WINDOWEVENT &&
            event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {

            target->windowWidth(event->window.data1);
            target->windowHeight(event->window.data2);
            target->notify(target->virtualWidth(), target->virtualHeight());
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
        // Ensure the values set in the constructor are updated before use in the initialization methods
        calculatedX_.update();
        calculatedY_.update();
        calculatedWidth_.update();
        calculatedHeight_.update();

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
        if(deviceWidth_ != value)
        {
            deviceWidth_ = value;
            recalculateWindow();
            SDL_SetWindowSize(mainWindow_.get(), deviceWidth_, deviceHeight_);
        }
    }
    int Screen::windowHeight() const {
        return deviceHeight_;
    }

    void Screen::windowHeight(int value) {
        if(deviceHeight_ != value)
        {
            deviceHeight_ = value;
            recalculateWindow();
            recalculateWindow();
            SDL_SetWindowSize(mainWindow_.get(), deviceWidth_, deviceHeight_);
        }
    }

    geColor Screen::backgroundColor() const {
        return background_.get();
    }

    void Screen::backgroundColor(geColor value) {
        background_ = value;
    }

    int Screen::virtualWidth() const {
        return virtualWidth_;
    }
    void Screen::virtualWidth(int value) {
        if(virtualWidth_ != value)
        {
            virtualWidth_ = value;
            recalculateWindow();
            notify(virtualWidth_, virtualHeight_);
        }
    }

    int Screen::virtualHeight() const {
        return virtualHeight_;
    }

    void Screen::virtualHeight(int value) {
        if(virtualHeight_ != value)
        {
            virtualHeight_ = value;
            recalculateWindow();
            notify(virtualWidth_, virtualHeight_);
        }
    }

    bool Screen::pixelPerfect() const {
        return pixelPerfect_;
    }

    void Screen::pixelPerfect(bool value) {
        pixelPerfect_ = value;
    }

    int Screen::calculatedX() const {
        return calculatedX_.get();
    }

    int Screen::calculatedY() const {
        return calculatedY_.get();
    }

    int Screen::calculatedWidth() const {
        return calculatedWidth_.get();
    }

    int Screen::calculatedHeight() const {
        return calculatedHeight_.get();
    }

    void Screen::recalculateWindow() {
        //Calculate the real size of the virtual screen when it will render
        float targetAspectRatio = (float) virtualWidth_ / virtualHeight_;

        // figure out the largest area that fits in this resolution at the desired aspect ratio
        int calculatedWidth = deviceWidth_;
        int calculatedHeight = std::lround(calculatedWidth / targetAspectRatio + 0.5f);

        if (calculatedHeight > deviceHeight_) {
            //It doesn't fit our height, we must switch to pillarbox then
            calculatedHeight = deviceHeight_;
            calculatedWidth = std::lround(calculatedHeight * targetAspectRatio + 0.5f);
        }

        // set up the new viewport centered in the backbuffer
        calculatedX_ = (deviceWidth_ / 2) - (calculatedWidth / 2);
        calculatedY_ = (deviceHeight_ / 2) - (calculatedHeight / 2);
        calculatedWidth_ = calculatedWidth;
        calculatedHeight_ = calculatedHeight;
    }

    void Screen::initGlAttributes()
    {
        glViewport(calculatedX_.get(), calculatedY_.get(), calculatedWidth_.get(), calculatedHeight_.get());
        CheckGlError();
        // Decide GL+GLSL versions
#if _WIN32
        // GL 3.0 + GLSL 130
        glslVersion_ = ShaderVersion::V330;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // TODO: review If it's necessary to specify a version or not
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif
        CheckGlError();

#if DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        CheckGlError();
        CheckSDLError();

        if(mainWindow_)
        {
            mainContext_ = SDL_GL_CreateContext(mainWindow_.get());
            makeCurrentContext();
        }

        // This makes our buffer swap synchronized with the monitor's vertical refresh
        // Enable vsync
        SDL_GL_SetSwapInterval(1);
        CheckSDLError();

        glewExperimental = GL_TRUE;
        glewInit();
        CheckGlError();

        //Initialize clear color
        glClearColor(background_.get().r, background_.get().g, background_.get().b, 1.f);
        CheckGlError();

        const char *glslVersionChar = (const char *)glGetString( GL_SHADING_LANGUAGE_VERSION );
        std::string glslVersion(glslVersionChar);

        GLint major, minor; 
        sscanf((const char*)glGetString(GL_VERSION), "%d.%d", &major, &minor);

#if DEBUG
        const GLubyte *renderer = glGetString( GL_RENDERER ); 
        const GLubyte *vendor = glGetString( GL_VENDOR ); 
        const GLubyte *version = glGetString( GL_VERSION ); 

        printf("[SCREEN] GL Vendor            : %s\n", vendor); 
        printf("[SCREEN] GL Renderer          : %s\n", renderer); 
        printf("[SCREEN] GL Version (string)  : %s\n", version); 
        printf("[SCREEN] GL Version (integer) : %d.%d\n", major, minor); 
        printf("[SCREEN] GLSL Version         : %s\n", glslVersion);
#endif

        if( glslVersion == "1.20")
            glslVersion_ = ShaderVersion::V120;
        if( glslVersion == "3.30")
            glslVersion_ = ShaderVersion::V330;
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
    if(calculatedX_.update() || calculatedY_.update() || calculatedHeight_.update() || calculatedWidth_.update())
        glViewport(calculatedX_.get(), calculatedY_.get(), calculatedWidth_.get(), calculatedHeight_.get());

    if(mainWindow_ && title_.update())
        SDL_SetWindowTitle(mainWindow_.get(), title_.get().c_str());

    if(background_.update())
    {
        glClearColor(background_.get().r, background_.get().g, background_.get().b, 1.f);
    }
}

Vec2D Screen::transformWindowToScreen(const Vec2D &position)
{
    Vec2D scale = Vec2D((float)virtualWidth_ / calculatedWidth_.get(),
                        (float)virtualHeight_ / calculatedHeight_.get()
                        );
    Vec2D normCalcPoint = Vec2D(
        (position.x - calculatedX_.get()) * scale.x,
        (position.y - calculatedY_.get()) * scale.y
    );

    return normCalcPoint;
}

ShaderVersion Screen::getGlslVersion() const{
    return glslVersion_;
}

}
}