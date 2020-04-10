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
#include "InputManager.hpp"

namespace GameEngine {
    class geGame;
    typedef std::shared_ptr<geGame> geGameRef;

    class geGameObject;
    typedef std::shared_ptr<geGameObject> geGameObjectRef;

    class PUBLICAPI geGame {
    public:
        virtual ~geGame() = 0;
        virtual void init() = 0;
        virtual int loop() = 0;
        virtual void shutdown() = 0;

        static geGameRef createInstance(const std::shared_ptr<geEnvironment> &env);
        virtual geGameObjectRef createObject(const std::string &name) = 0;
        virtual geGameObjectRef createFromPrototype(const std::string &prototype) = 0;
        virtual geGameObjectRef findObjectByNameInCurrentScene(const std::string &gameObjectName) = 0;
        virtual void changeScene(const std::string &name) = 0;
        virtual std::weak_ptr<geCamera> cameraOfCurrentScene() const = 0;

        virtual std::shared_ptr<geScreen> screen() const = 0;
        virtual std::shared_ptr<geAudio> audio() const = 0;
        virtual std::shared_ptr<InputManager> input() const = 0;
    };
}

#endif //SPACEINVADERS_GEGAME_HPP
