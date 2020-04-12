//
// Created by adria on 23/11/2019.
//

#ifndef GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP
#define GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP


#include <vector>
#include <game-engine/properties/Property.hpp>
#include <game-engine/geEnvironment.hpp>

class PropertyData;
class GameComponentsProvider
{
    std::vector<std::string> namesCached_;
    GameEngine::geEnvironmentRef env_;

    void updateNames();
    std::shared_ptr<PropertyData> buildPropertyByType(const GameEngine::PropertyBase &property) const;
public:
    GameComponentsProvider();

    std::vector<std::string> getRegisteredPropertiesIds();

    std::vector<std::shared_ptr<PropertyData>> getPropertiesMetadata(const std::string &name) const;
    std::vector<std::shared_ptr<PropertyData>> getPropertiesMetadataByComponent(const std::string &componentName) const;
};


#endif //GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP
