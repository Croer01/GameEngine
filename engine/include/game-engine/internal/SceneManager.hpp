//
// Created by adria on 01/10/2018.
//

#ifndef GAMEENGINE_SCENEMANAGER_HPP
#define GAMEENGINE_SCENEMANAGER_HPP


#include <game-engine/api.hpp>
#include <memory>
#include <unordered_map>
#include <game-engine/internal/Scene.hpp>

namespace GameEngine {

class Game;

namespace Internal {

    class PUBLICAPI SceneManager {
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        std::shared_ptr<Scene> currentScene_;
        std::string currentSceneName_;
        std::string sceneNameToChange_;
        Game *game_;
    public:
        void update(float elapsedTime);

        void registerScene(const std::string &name, const std::string &filename);

        void changeScene(const std::string &name);

        void changeSceneInSafeMode();

        void addObjectIntoCurrentScene(const std::shared_ptr<Internal::GameObject> &object);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName);

        void reloadScene();

        void clear();

        bool isSceneLoaded() const;

        geCamera *getCameraOfCurrentScene() const;

        void bindGame(Game *game);
        void unbindGame();
        geDataRef saveCurrentSceneState() const;
        void removeDestroyedObjects();
    };
}
}
#endif //GAMEENGINE_SCENEMANAGER_HPP
