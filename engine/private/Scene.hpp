//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENE_HPP
#define SPACEINVADERS_SCENE_HPP


#include <string>
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "graphics/Camera.hpp"
#include <game-engine/geData.hpp>

namespace GameEngine {
namespace Internal {

    class Game;

    class Scene {
        std::string filePath_;
        std::string name_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;
        std::shared_ptr<Camera> cam_;

        void loadFile(geGame *game);

    public:
        explicit Scene(const std::string &filePath);

        void init(geGame *game);

        void update(float elapsedTime);

        void addGameObject(const std::shared_ptr<GameObject> &gameObject);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName) const;

        void shutDown();

        std::shared_ptr<GameEngine::geCamera> cam() const;

        geDataRef saveCurrentState() const;

        void removeDestroyedObjects();
    };
}
}

#endif //SPACEINVADERS_SCENE_HPP
