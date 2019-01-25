//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENEMANAGER_HPP
#define SPACEINVADERS_SCENEMANAGER_HPP


#include <memory>
#include <unordered_map>
#include "Singleton.hpp"
#include "Scene.hpp"
namespace GameEngine {
namespace Internal {
    class SceneManager : public Singleton<SceneManager> {
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        std::shared_ptr<Scene> currentScene_;
        std::string currentSceneName_;
        std::string sceneNameToChange_;
    public:
        void update(float elapsedTime);

        void registerScene(const std::string &name, const std::string &filename);

        void changeScene(const std::string &name);

        void changeSceneInSafeMode();

        std::shared_ptr<GameObject> createGameObject(const std::string &objectType);

        std::shared_ptr<GameObject> findObjectByName(const std::string &gameObjectName);

        void reloadScene();
    };
}
}
#endif //SPACEINVADERS_SCENEMANAGER_HPP
