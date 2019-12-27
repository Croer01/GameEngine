//
// Created by adria on 23/11/2019.
//

#include <game-engine/geEnvironment.hpp>
#include "GameComponentsProvider.hpp"

void GameComponentsProvider::updateNames()
{
    GameEngine::geEnvironment env;
    namesCached_ = env.getRegisteredPropertiesIds();
}

std::vector<std::string> GameComponentsProvider::getRegisteredPropertiesIds()
{
    if(namesCached_.empty())
        updateNames();

    return namesCached_;
}

std::vector<PropertyDataRef> GameComponentsProvider::getPropertiesMetadata(const std::string &name) const
{
    GameEngine::geEnvironment env;
    std::shared_ptr<GameEngine::PropertySetBase> gameProperties = env.getProperties(name);

    std::vector<PropertyDataRef> properties;
    properties.reserve(gameProperties->size());
    for(int i = 0; i < gameProperties->size(); i++)
    {
        properties.emplace_back(buildPropertyByType(gameProperties->get(i)));
    }

    return properties;
}

PropertyDataRef GameComponentsProvider::buildPropertyByType(const GameEngine::PropertyBase &property) const
{
    PropertyDataType result = PropertyDataType::STRING;
    PropertyDataRef propertyData;
    switch (property.type())
    {
        case GameEngine::PropertyTypes::INT:
            propertyData = PropertyDataRef(new PropertyIntData(property.name()));
           break;
        case GameEngine::PropertyTypes::FLOAT:
            propertyData = PropertyDataRef(new PropertyFloatData(property.name()));
           break;
        case GameEngine::PropertyTypes::STRING:
            propertyData = PropertyDataRef(new PropertyStringData(property.name()));
           break;
        case GameEngine::PropertyTypes::BOOL:
            propertyData = PropertyDataRef(new PropertyBoolData(property.name()));
           break;
        case GameEngine::PropertyTypes::VEC2D:
            propertyData = PropertyDataRef(new PropertyVec2DData(property.name()));
           break;
        case GameEngine::PropertyTypes::ARRAY_STRING:
            propertyData = PropertyDataRef(new PropertyStringArrayData(property.name()));
           break;
        case GameEngine::PropertyTypes::ARRAY_VEC2D:
            propertyData = PropertyDataRef(new PropertyVec2DArrayData(property.name()));
           break;
        case GameEngine::PropertyTypes::COLOR:
            propertyData = PropertyDataRef(new PropertyColorData(property.name()));
           break;
        case GameEngine::PropertyTypes::UNKNOWN:
            throw std::invalid_argument("property " + property.name() + " has unknown type");
            break;
    }
    return propertyData;
}
