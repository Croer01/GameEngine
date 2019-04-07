//
// Created by adria on 25/01/2019.
//

#ifndef SPACEINVADERS_GEGAME_HPP
#define SPACEINVADERS_GEGAME_HPP

#include <game-engine/api.hpp>
#include <game-engine/geEnvironment.hpp>
#include <string>
#include <memory>
#include "geScreen.hpp"
#include "geAudio.hpp"
#include "geCamera.hpp"

namespace GameEngine {
    class geGame;
    class geGameObject;
    typedef std::shared_ptr<geGameObject> geGameObjectRef;

    class PUBLICAPI geGame {
        geEnvironment environment_;
        std::unique_ptr<geAudio> audio_;
        bool initialized_;
    public:
        geGame();
        virtual ~geGame();
        void init();
        int loop();
        void shutdown();

        geGameObjectRef createObject(const std::string &name);
        geGameObjectRef createFromPrototype(const std::string &prototype);
        void configEnvironment(const geEnvironment &environment);
        geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName);
        void changeScene(const std::string &name);
        std::weak_ptr<geCamera> cameraOfCurrentScene() const;

        geScreen &screen() const;
        geAudio &audio() const;
    };
}

#endif //SPACEINVADERS_GEGAME_HPP
