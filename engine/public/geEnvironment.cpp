//
// Created by adria on 27/01/2019.
//

#include <game-engine/geEnvironment.hpp>
#include "../private/ObjectManager.hpp"
#include "../private/SceneManager.hpp"
#include "../private/Game.hpp"

namespace GameEngine {
geEnvironment::geEnvironment() : configurationPath_("conf")
{
}

void GameEngine::geEnvironment::configurationPath(const std::string &config)
{
    configurationPath_ = config;
}

std::string geEnvironment::configurationPath() const
{
    return configurationPath_;
}

void geEnvironment::firstScene(const std::string &sceneName)
{
    firstScene_ = sceneName;
}

std::string geEnvironment::firstScene() const
{
    return firstScene_;
}

void geEnvironment::addPrototype(const std::string &name, const std::string &filePath)
{
    Internal::ObjectManager::GetInstance().registerPrototype(name, filePath);
}

void geEnvironment::addScene(const std::string &name, const std::string &filePath)
{
    Internal::SceneManager::GetInstance().registerScene(name, filePath);
}

void geEnvironment::registerComponent(const std::string &idType, ComponentBuilder *builder)
{
    Internal::ObjectManager::GetInstance().registerComponentBuilder(idType, builder);
}

void geEnvironment::registerProperties(const std::string &id, PropertyInstantiator *instantiator)
{
    PropertiesManager::GetInstance().registerPropertiesSetBuilder(id, std::shared_ptr<PropertyInstantiator>(instantiator));
}

std::vector<std::string> geEnvironment::getRegisteredPropertiesIds() const
{
    //TODO: Do this in a better way. Now is a workarround to force initialize properties
    GameEngine::Internal::Game::GetInstance();
    return PropertiesManager::GetInstance().getIds();
}

std::shared_ptr<PropertySetBase> geEnvironment::getProperties(const std::string &id) const
{
    return PropertiesManager::GetInstance().instantiate(id);
}
}

