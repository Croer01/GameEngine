//
// Created by adria on 25/09/2018.
//

#ifndef SPACEINVADERS_OBJECTMANAGER_HPP
#define SPACEINVADERS_OBJECTMANAGER_HPP


#include "Singleton.hpp"
#include "Factory.hpp"
#include <game-engine/geComponent.hpp>
#include <game-engine/Builder.hpp>
#include "GameObject.hpp"
#include <memory>
namespace GameEngine {
namespace Internal {
//declarations of the global methods generated to use in the ObjectManager

    class ObjectManager : public Singleton<ObjectManager> {
    private:
        Factory<geComponent, ComponentBuilder, std::string> componentFactory_;
        std::unordered_map<std::string, std::unique_ptr<GameObject>> prototypes_;
    public:
        void registerComponentBuilder(const std::string &idType, ComponentBuilder *builder);

        geComponentRef createComponent(const std::string &idType, const YAML::Node &data);

        void registerPrototype(const std::string &objectType, const std::string &filename);

        std::shared_ptr<GameObject> createGameObject(const std::string &objectType);

        void clear();
    };
}
}


#endif //SPACEINVADERS_OBJECTMANAGER_HPP
