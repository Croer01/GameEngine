//
// Created by adria on 26/01/2019.
//

#ifndef SPACEINVADERS_GEENVIRONMENT_HPP
#define SPACEINVADERS_GEENVIRONMENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/Builder.hpp>
#include <string>
#include <vector>
#include <game-engine/properties/PropertiesManager.hpp>

namespace GameEngine {
    class PUBLICAPI geEnvironment {
        std::string configurationPath_;
        std::string firstScene_;

    public:
        geEnvironment();
        virtual ~geEnvironment() = default;
        void registerComponent(const std::string &idType, ComponentBuilder *builder);
        std::shared_ptr<PropertySetBase> getProperties(const std::string &id) const;
        std::vector<std::string> getRegisteredPropertiesIds() const;
        void configurationPath(const std::string &config);
        std::string configurationPath() const;
        void firstScene(const std::string &sceneName);
        std::string firstScene() const;
        void addPrototype(const std::string &name, const std::string &filePath);
        void addScene(const std::string &name, const std::string &filePath);
    };
}


#endif //SPACEINVADERS_GEENVIRONMENT_HPP
