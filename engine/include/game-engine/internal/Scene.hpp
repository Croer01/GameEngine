//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENE_HPP
#define SPACEINVADERS_SCENE_HPP


#include <game-engine/api.hpp>
#include <string>
#include <vector>
#include <memory>
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/internal/graphics/Camera.hpp>
#include <game-engine/geData.hpp>

namespace GameEngine {

class Game;

namespace Internal {

    class PUBLICAPI Scene {
        std::string filePath_;
        std::string name_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;
        std::unique_ptr<Camera> cam_;
        bool preInitialized_;
        bool initialized_;

        void loadFile(Game *game);

    public:
        explicit Scene(const std::string &filePath);

        void init(Game *game);

        void update(float elapsedTime);

        void addGameObject(const std::shared_ptr<GameObject> &gameObject);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName) const;

        void shutDown();

        GameEngine::geCamera *cam() const;

        geDataRef saveCurrentState() const;

        void removeDestroyedObjects();
    };
}
}

#endif //SPACEINVADERS_SCENE_HPP
