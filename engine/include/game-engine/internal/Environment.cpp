//
// Created by adria on 06/04/2020.
//

#include <game-engine/components/SpriteComponent.hpp>
#include <game-engine/components/GeometryComponent.hpp>
#include <game-engine/components/SpriteAnimatedComponent.hpp>
#include <game-engine/components/TextComponent.hpp>
#include <game-engine/components/ui/UIButtonComponent.hpp>
#include <game-engine/components/ui/UITextInputComponent.hpp>
#include <game-engine/components/ui/UIPanelComponent.hpp>
#include <game-engine/components/ui/UIImageComponent.hpp>
#include <game-engine/components/ColliderComponent.hpp>
#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/components/ParticleEmitterComponent.hpp>
#include <game-engine/components/SpawnerComponent.hpp>
#include "Environment.hpp"
#include "game-engine/components/CarColliderComponent.hpp"

namespace fs = boost::filesystem;

namespace GameEngine
{
namespace Internal
{

Environment::Environment() : configurationPath_("conf"), dataPath_("data")
{
    objectManager_ = std::make_unique<ObjectManager>();
    //Register the engine's default components
    objectManager_->registerComponentBuilder("SpriteComponent", new ComponentTBuilder<SpriteComponent>());
    objectManager_->registerComponentBuilder("GeometryComponent", new ComponentTBuilder<GeometryComponent>());
    objectManager_->registerComponentBuilder("SpriteAnimatedComponent", new ComponentTBuilder<SpriteAnimatedComponent>());
    objectManager_->registerComponentBuilder("ColliderComponent", new ComponentTBuilder<ColliderComponent>());
    objectManager_->registerComponentBuilder("TextComponent", new ComponentTBuilder<TextComponent>());
    objectManager_->registerComponentBuilder("AudioComponent", new ComponentTBuilder<AudioComponent>());
    objectManager_->registerComponentBuilder("ParticleEmitterComponent", new ComponentTBuilder<ParticleEmitterComponent>());
    objectManager_->registerComponentBuilder("ParticlesPropertiesComponent", new ComponentTBuilder<ParticlesPropertiesComponent>());
    objectManager_->registerComponentBuilder("SpawnerComponent", new ComponentTBuilder<SpawnerComponent>());
    objectManager_->registerComponentBuilder("CarColliderComponent", new ComponentTBuilder<CarColliderComponent>());

    //GUI Components
    objectManager_->registerComponentBuilder("UIButtonComponent", new ComponentTBuilder<UIButtonComponent>());
    objectManager_->registerComponentBuilder("UITextComponent", new ComponentTBuilder<UITextComponent>());
    objectManager_->registerComponentBuilder("UITextInputComponent", new ComponentTBuilder<UITextInputComponent>());
    objectManager_->registerComponentBuilder("UIPanelComponent", new ComponentTBuilder<UIPanelComponent>());
    objectManager_->registerComponentBuilder("UIImageComponent", new ComponentTBuilder<UIImageComponent>());

    sceneManager_ = std::make_unique<SceneManager>();
}

void Environment::configurationPath(const std::string &config)
{
    configurationPath_ = config;
}

std::string Environment::configurationPath() const
{
    return configurationPath_;
}

void Environment::dataPath(const std::string &data)
{
    dataPath_ = data;
}

std::string Environment::dataPath() const
{
    return dataPath_;
}

void Environment::firstScene(const std::string &sceneName)
{
    firstScene_ = sceneName;
}

std::string Environment::firstScene() const
{
    return firstScene_;
}

void Environment::addPrototype(const std::string &name, const std::string &filePath)
{
    objectManager_->registerPrototype(name, dataPath_ + "/" + filePath);
}

void Environment::addScene(const std::string &name, const std::string &filePath)
{
    sceneManager_->registerScene(name, dataPath_ + "/" + filePath);
}

void Environment::registerComponent(const std::string &idType, ComponentBuilder *builder)
{
    objectManager_->registerComponentBuilder(idType, builder);
}

std::vector<std::string> Environment::getRegisteredPropertiesIds() const
{
    return objectManager_->getComponentIds();
}

std::shared_ptr<PropertySetBase> Environment::getProperties(const std::string &id) const
{
    return objectManager_->createProperties(id);
}

SceneManager *Environment::sceneManager() const
{
    return sceneManager_.get();
}

ObjectManager *Environment::objectManager() const
{
    return objectManager_.get();
}

void Environment::setMakeCurrentContextCallback(const MakeCurrentContextCallback &callback)
{
    makeCurrentContextCallback_ = callback;
}

MakeCurrentContextCallback Environment::getMakeCurrentContextCallback() const
{
    return makeCurrentContextCallback_;
}

void Environment::addResourcesFromPath(const std::string &dataPath)
{
    const boost::filesystem::path &directoryPath = fs::path(dataPath);
    recursiveDataFilesRegister(directoryPath);
}

void Environment::recursiveDataFilesRegister(const boost::filesystem::path &directoryPath)
{
    if(fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        fs::recursive_directory_iterator end;
        for (fs::recursive_directory_iterator itr(directoryPath); itr != end; ++itr)
        {
            const fs::path &path = itr->path();
            if (is_regular_file(path))
            {
                if(path.extension() == ".prototype")
                {
                    objectManager_->registerPrototype(path.stem().string(), path.string());
                }
                else if(path.extension() == ".scene")
                {
                    sceneManager_->registerScene(path.stem().string(), path.string());
                }
            }
        }
    }
}
}
}
