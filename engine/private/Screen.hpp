//
// Created by adria on 28/10/2018.
//

#ifndef SPACEINVADERS_SCREEN_HPP
#define SPACEINVADERS_SCREEN_HPP


#include <game-engine/geScreen.hpp>
#include <string>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <memory>

namespace GameEngine {
namespace Internal {
    int resizingEventWatcher(void* data, SDL_Event* event);

    class Screen : public geScreen {
        //The size of the window or screen if window is fullscreen
        int deviceWidth_;
        int deviceHeight_;

        //dimensions of the virtual screen. This is used to preserve always the same size independently of the device resolution
        int virtualWidth_;
        int virtualHeight_;

        int calculatedX_;
        int calculatedY_;
        int calculatedWidth_;
        int calculatedHeight_;

        std::string title_;
        geColor background_;
        bool pixelPerfect_;
        bool allowResize_;

        // window stuff
        std::shared_ptr<SDL_Window> mainWindow_;
        SDL_GLContext mainContext_;
    public:

        explicit Screen(const std::string &filePath, bool embedded);
        virtual ~Screen();
        virtual std::string title() const;
        virtual void title(const std::string &value);
        virtual int windowWidth() const;
        virtual void windowWidth(int value);
        virtual int windowHeight() const;
        virtual void windowHeight(int value);
        virtual int virtualWidth() const;
        virtual void virtualWidth(int value);
        virtual int virtualHeight() const;
        virtual void virtualHeight(int value);
        virtual geColor backgroundColor() const;
        virtual void backgroundColor(geColor value);
        virtual bool pixelPerfect() const;
        virtual void pixelPerfect(bool value);
        virtual bool resizable() const;
        virtual void resizable(bool value);

        int calculatedX() const;
        int calculatedY() const;
        int calculatedWidth() const;
        int calculatedHeight() const;

        void makeCurrentContext();
        void swapWindow();
        SDL_Window *sdlWindow() const;
    private:
        void recalculateWindow();
        void initSDLWindow();
        void initGlAttributes();

    };
}
}
#endif //SPACEINVADERS_SCREEN_HPP
