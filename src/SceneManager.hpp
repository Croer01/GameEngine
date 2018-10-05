//
// Created by adria on 01/10/2018.
//

#ifndef SPACEINVADERS_SCENEMANAGER_HPP
#define SPACEINVADERS_SCENEMANAGER_HPP


#include <memory>
#include <unordered_map>
#include "Singleton.hpp"
#include "Scene.hpp"

class SceneManager : public Singleton<SceneManager>{
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
public:
    void registerScene(const std::string &name, const std::string &filename);
    std::shared_ptr<Scene> getScene(const std::string &name);
};


#endif //SPACEINVADERS_SCENEMANAGER_HPP
