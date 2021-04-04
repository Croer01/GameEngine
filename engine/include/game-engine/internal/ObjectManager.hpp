//
// Created by adria on 25/09/2018.
//

#ifndef GAMEENGINE_OBJECTMANAGER_HPP
#define GAMEENGINE_OBJECTMANAGER_HPP


#include <game-engine/internal/Factory.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/Builder.hpp>
#include <memory>

namespace GameEngine {

class Game;

namespace Internal {

    class GameObject;

    class ObjectManager
    {
        Factory<geComponent, ComponentBuilder, std::string> componentFactory_;
        std::unordered_map<std::string, std::unique_ptr<GameObject>> prototypes_;

    public:
        ~ObjectManager();

        void registerComponentBuilder(const std::string &idType, ComponentBuilder *builder);

        geComponentRef createComponent(const std::string &idType);
        geComponentRef createComponent(const std::string &idType, const YAML::Node &data);
        ComponentDataRef createProperties(const std::string &idType);

        void registerPrototype(const std::string &objectType, const std::string &filename);

        std::shared_ptr<GameObject> createGameObject(const std::string &objectType, Game *game);

        std::vector<std::string> getComponentIds() const;
    };
}
}


#endif //GAMEENGINE_OBJECTMANAGER_HPP
