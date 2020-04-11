//
// Created by adria on 26/01/2019.
//

#ifndef SPACEINVADERS_GEENVIRONMENT_HPP
#define SPACEINVADERS_GEENVIRONMENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/Builder.hpp>
#include <string>
#include <vector>

namespace GameEngine {
    class geEnvironment;
    typedef std::shared_ptr<geEnvironment> geEnvironmentRef;

    class PUBLICAPI geEnvironment {
    public:
        virtual ~geEnvironment() = 0;
        static geEnvironmentRef createInstance();
        virtual void registerComponent(const std::string &idType, ComponentBuilder *builder) = 0;
        virtual std::shared_ptr<PropertySetBase> getProperties(const std::string &id) const = 0;
        virtual std::vector<std::string> getRegisteredPropertiesIds() const = 0;
        virtual void configurationPath(const std::string &config) = 0;
        virtual std::string configurationPath() const = 0;
        virtual void firstScene(const std::string &sceneName) = 0;
        virtual std::string firstScene() const = 0;
        virtual void addPrototype(const std::string &name, const std::string &filePath) = 0;
        virtual void addScene(const std::string &name, const std::string &filePath) = 0;
        virtual void setGameEmbedded(bool embedded) = 0;
        virtual bool isGameEmbedded() const = 0;
    };
}


#endif //SPACEINVADERS_GEENVIRONMENT_HPP
