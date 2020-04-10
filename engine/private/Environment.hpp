//
// Created by adria on 06/04/2020.
//

#ifndef GAMEENGINE_ENVIRONMENT_HPP
#define GAMEENGINE_ENVIRONMENT_HPP

#include <game-engine/geEnvironment.hpp>
#include "ObjectManager.hpp"
#include "SceneManager.hpp"

namespace GameEngine
{
namespace Internal
{

class Environment : public geEnvironment
{
    std::string configurationPath_;
    std::string firstScene_;
    std::unique_ptr<ObjectManager> objectManager_;
    std::unique_ptr<SceneManager> sceneManager_;

public:
    Environment();
    virtual ~Environment(){};
    virtual void registerComponent(const std::string &idType, ComponentBuilder *builder);
    virtual std::shared_ptr<PropertySetBase> getProperties(const std::string &id) const;
    virtual std::vector<std::string> getRegisteredPropertiesIds() const;
    virtual void configurationPath(const std::string &config);
    virtual std::string configurationPath() const;
    virtual void firstScene(const std::string &sceneName);
    virtual std::string firstScene() const;
    virtual void addPrototype(const std::string &name, const std::string &filePath);
    virtual void addScene(const std::string &name, const std::string &filePath);

    SceneManager &sceneManager() const;
    ObjectManager &objectManager() const;
};

}
}

#endif //GAMEENGINE_ENVIRONMENT_HPP