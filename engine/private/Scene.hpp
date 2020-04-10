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

namespace GameEngine {
namespace Internal {

    class Game;

    class Scene {
        std::string filename_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;
        std::shared_ptr<Camera> cam_;

        void loadFile(const std::shared_ptr<Game> &game);

    public:
        explicit Scene(const std::string &filename);

        void init(const std::shared_ptr<Game> &game);

        void update(float elapsedTime);

        void addGameObject(const std::shared_ptr<GameObject> &gameObject);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName) const;

        void shutDown();

        std::shared_ptr<GameEngine::geCamera> cam() const;
    };
}
}

#endif //SPACEINVADERS_SCENE_HPP
