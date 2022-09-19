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
#include <game-engine/internal/Environment.hpp>
#include <game-engine/components/CarColliderComponent.hpp>

#if defined(_WIN32)
    #include <windows.h>
#endif

namespace fs = std::filesystem;

namespace GameEngine
{
namespace Internal
{
// Get the path of the game executable.
// Code based on https://gist.github.com/Jacob-Tate/7b326a086cf3f9d46e32315841101109
std::string getGameAbsolutePath()
{
    fs::path exePath;
    #if defined(_MSC_VER)
        wchar_t path[FILENAME_MAX] = { 0 };
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        exePath = std::filesystem::path(path);
    #else
        exePath = std::filesystem::canonical("/proc/self/exe");
    #endif

    return exePath.parent_path().string() + "/";
}

std::string Environment::getAbsoluteDataPath() const{
    return getGameAbsolutePath() + dataPath_ + "/";
}

Environment::Environment() : configurationPath_("conf"), dataPath_("data")
{
    objectManager_ = std::make_unique<ObjectManager>();
    //Register the engine's default components
    objectManager_->registerComponentBuilder("SpriteComponent", CreateComponentBuilder<SpriteComponent>());
    objectManager_->registerComponentBuilder("GeometryComponent", CreateComponentBuilder<GeometryComponent>());
    objectManager_->registerComponentBuilder("SpriteAnimatedComponent", CreateComponentBuilder<SpriteAnimatedComponent>());
    objectManager_->registerComponentBuilder("ColliderComponent", CreateComponentBuilder<ColliderComponent>());
    objectManager_->registerComponentBuilder("TextComponent", CreateComponentBuilder<TextComponent>());
    objectManager_->registerComponentBuilder("AudioComponent", CreateComponentBuilder<AudioComponent>());
    objectManager_->registerComponentBuilder("ParticleEmitterComponent", CreateComponentBuilder<ParticleEmitterComponent>());
    objectManager_->registerComponentBuilder("ParticlesPropertiesComponent", CreateComponentBuilder<ParticlesPropertiesComponent>());
    objectManager_->registerComponentBuilder("SpawnerComponent", CreateComponentBuilder<SpawnerComponent>());
    objectManager_->registerComponentBuilder("CarColliderComponent", CreateComponentBuilder<CarColliderComponent>());

    //GUI Components
    objectManager_->registerComponentBuilder("UIButtonComponent", CreateComponentBuilder<UIButtonComponent>());
    objectManager_->registerComponentBuilder("UITextComponent", CreateComponentBuilder<UITextComponent>());
    objectManager_->registerComponentBuilder("UITextInputComponent", CreateComponentBuilder<UITextInputComponent>());
    objectManager_->registerComponentBuilder("UIPanelComponent", CreateComponentBuilder<UIPanelComponent>());
    objectManager_->registerComponentBuilder("UIImageComponent", CreateComponentBuilder<UIImageComponent>());

    sceneManager_ = std::make_unique<SceneManager>();
    globalData_ =  std::make_unique<GlobalData>();
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
    objectManager_->registerPrototype(name, getGameAbsolutePath() + dataPath_ + "/" + filePath);
}

void Environment::addScene(const std::string &name, const std::string &filePath)
{
    sceneManager_->registerScene(name, getGameAbsolutePath() + dataPath_ + "/" + filePath);
}

void Environment::registerComponent(const std::string &idType, ComponentBuilder *builder)
{
    objectManager_->registerComponentBuilder(idType, builder);
}

std::vector<std::string> Environment::getRegisteredPropertiesIds() const
{
    return objectManager_->getComponentIds();
}

ComponentDataRef Environment::getProperties(const std::string &id) const
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

GlobalData* Environment::getGlobalData(){
    return globalData_.get();
}

void Environment::addResourcesFromPath(const std::string &dataPath)
{
    const fs::path &directoryPath = fs::path(getGameAbsolutePath() + dataPath);
    recursiveDataFilesRegister(directoryPath);
}

void Environment::recursiveDataFilesRegister(const std::filesystem::path &directoryPath)
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
