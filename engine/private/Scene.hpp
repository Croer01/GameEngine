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
#include "pathfinding/AStarPathfinding.hpp"

namespace GameEngine {
namespace Internal {

    class Game;

    class Scene {
        struct PathfindingConfig
        {
            int columns;
            int rows;
        };

        std::string filename_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;
        std::shared_ptr<Camera> cam_;
        std::unique_ptr<PathfindingConfig> pathfindingConfig_;
        std::unique_ptr<AStarPathfinding> pathfinding_;

        void loadFile(Game *game);

    public:
        explicit Scene(const std::string &filename);

        void init(Game *game);

        void update(float elapsedTime);

        void addGameObject(const std::shared_ptr<GameObject> &gameObject);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName) const;

        void shutDown();

        std::shared_ptr<GameEngine::geCamera> cam() const;

        AStarPathfinding *getPathfinding() const;
    };
}
}

#endif //SPACEINVADERS_SCENE_HPP
