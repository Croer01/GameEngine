//
// Created by adria on 26/01/2019.
//

#ifndef SPACEINVADERS_GEENVIRONMENT_HPP
#define SPACEINVADERS_GEENVIRONMENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/components/ComponentData.hpp>
#include <game-engine/Builder.hpp>
#include <string>
#include <vector>
#include <functional>

namespace GameEngine
{

typedef std::function<void()> MakeCurrentContextCallback;

class geEnvironment;
typedef std::shared_ptr<geEnvironment> geEnvironmentRef;

class PUBLICAPI geEnvironment
{
public:
    virtual ~geEnvironment() = 0;
    static geEnvironmentRef createInstance();
    virtual void registerComponent(const std::string &idType, ComponentBuilder *builder) = 0;
    virtual ComponentDataRef getProperties(const std::string &id) const = 0;
    virtual std::vector<std::string> getRegisteredPropertiesIds() const = 0;
    virtual void configurationPath(const std::string &config) = 0;
    virtual std::string configurationPath() const = 0;
    virtual void dataPath(const std::string &data) = 0;
    virtual std::string dataPath() const = 0;
    virtual void firstScene(const std::string &sceneName) = 0;
    virtual std::string firstScene() const = 0;
    virtual void addPrototype(const std::string &name, const std::string &filePath) = 0;
    virtual void addScene(const std::string &name, const std::string &filePath) = 0;
    virtual void addResourcesFromPath(const std::string &dataPath) = 0;
    // this automatically configure game instance to render in an image (GL FrameBufferObject)
    // use geGame::getRenderer() to get the gl texture's id
    virtual void setMakeCurrentContextCallback(const MakeCurrentContextCallback &callback) = 0;
};
}


#endif //SPACEINVADERS_GEENVIRONMENT_HPP
