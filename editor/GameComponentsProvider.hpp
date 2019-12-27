//
// Created by adria on 23/11/2019.
//

#ifndef GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP
#define GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP


#include <vector>
#include <game-engine/properties/Property.hpp>
#include "ViewModels.hpp"

class GameComponentsProvider
{
    std::vector<std::string> namesCached_;

    void updateNames();
    PropertyDataRef buildPropertyByType(const GameEngine::PropertyBase &property) const;
public:

    std::vector<std::string> getRegisteredPropertiesIds();

    std::vector<PropertyDataRef> getPropertiesMetadata(const std::string &name) const;
};


#endif //GAMEENGINEEDITOR_GAMECOMPONENTSPROVIDER_HPP
