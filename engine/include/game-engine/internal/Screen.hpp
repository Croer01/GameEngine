//
// Created by adria on 28/10/2018.
//

#ifndef GAMEENGINE_SCREEN_HPP
#define GAMEENGINE_SCREEN_HPP


#include <game-engine/api.hpp>
#include <game-engine/geScreen.hpp>
#include <string>
#include <SDL_video.h>
#include <SDL_events.h>
#include <memory>
#include <game-engine/internal/DelayedSetter.hpp>
#include <game-engine/events/Subject.hpp>

namespace GameEngine {
namespace Internal {
    int resizingEventWatcher(void* data, SDL_Event* event);

    class PUBLICAPI Screen : public geScreen, public Subject<int, int>{
        //The size of the window or screen if window is fullscreen
        int deviceWidth_;
        int deviceHeight_;

        //dimensions of the virtual screen. This is used to preserve always the same size independently of the device resolution
        int virtualWidth_;
        int virtualHeight_;

        Vec2D windowRelativePos_;
        DelayedSetter<int> calculatedX_;
        DelayedSetter<int> calculatedY_;
        DelayedSetter<int> calculatedWidth_;
        DelayedSetter<int> calculatedHeight_;

        DelayedSetter<std::string> title_;
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
        virtual Vec2D transformWindowToScreen(const Vec2D &position);

        int calculatedX() const;
        int calculatedY() const;
        int calculatedWidth() const;
        int calculatedHeight() const;

        void makeCurrentContext();
        void swapWindow();
        SDL_Window *sdlWindow() const;

        void update();
    private:
        void recalculateWindow();
        void initSDLWindow();
        void initGlAttributes();

    };
}
}
#endif //GAMEENGINE_SCREEN_HPP
