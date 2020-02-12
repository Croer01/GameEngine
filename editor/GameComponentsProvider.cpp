//
// Created by adria on 23/11/2019.
//

#include <game-engine/geEnvironment.hpp>
#include "GameComponentsProvider.hpp"
#include "ViewModels.hpp"

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

std::vector<std::shared_ptr<PropertyData>>
GameComponentsProvider::getPropertiesMetadataByComponent(const std::string &componentName) const
{
    GameEngine::geEnvironment env;
    std::shared_ptr<GameEngine::PropertySetBase> gameProperties = env.getProperties(componentName);

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
        case GameEngine::PropertyTypes::INT: {
            PropertyIntData *propertyInt = new PropertyIntData(property.name());
            propertyData = PropertyDataRef(propertyInt);
            property.getDefault(&propertyInt->value_);
        }
           break;
        case GameEngine::PropertyTypes::FLOAT: {
            PropertyFloatData *propertyFloat = new PropertyFloatData(property.name());
            propertyData = PropertyDataRef(propertyFloat);
            property.getDefault(&propertyFloat->value_);
        }
           break;
        case GameEngine::PropertyTypes::STRING: {
            PropertyStringData *propertyString = new PropertyStringData(property.name());
            propertyData = PropertyDataRef(propertyString);
            property.getDefault(&propertyString->value_);
        }
           break;
        case GameEngine::PropertyTypes::BOOL: {
            PropertyBoolData *propertyBool = new PropertyBoolData(property.name());
            propertyData = PropertyDataRef(propertyBool);
            property.getDefault(&propertyBool->value_);
        }
           break;
        case GameEngine::PropertyTypes::VEC2D: {
            PropertyVec2DData *propertyVec2D = new PropertyVec2DData(property.name());
            propertyData = PropertyDataRef(propertyVec2D);
            GameEngine::Vec2D defaultValue;
            property.getDefault(&defaultValue);
            propertyVec2D->value_.xy = {defaultValue.x, defaultValue.y};
        }
           break;
        case GameEngine::PropertyTypes::ARRAY_STRING: {
            PropertyStringArrayData *propertyStringArray = new PropertyStringArrayData(property.name());
            propertyData = PropertyDataRef(propertyStringArray);
            property.getDefault(&propertyStringArray->value_);
        }
           break;
        case GameEngine::PropertyTypes::ARRAY_VEC2D: {
            PropertyVec2DArrayData *propertyVec2DArray = new PropertyVec2DArrayData(property.name());
            propertyData = PropertyDataRef(propertyVec2DArray);
            std::vector<GameEngine::Vec2D> defaultValue;
            property.getDefault(&defaultValue);
            for(const auto &value : defaultValue)
            {
                Vector2DData data;
                data.xy = {value.x, value.y};
                propertyVec2DArray->value_.push_back(data);
            }
        }
           break;
        case GameEngine::PropertyTypes::COLOR: {
            PropertyColorData *propertyColor = new PropertyColorData(property.name());
            propertyData = PropertyDataRef(propertyColor);
            GameEngine::geColor defaultValue;
            property.getDefault(&defaultValue);
            propertyColor->value_.rgb = {defaultValue.r, defaultValue.g, defaultValue.b};
        }
           break;
        case GameEngine::PropertyTypes::FILEPATH: {
            PropertyFilePathData *propertyFilepath = new PropertyFilePathData(property.name());
            propertyData = PropertyDataRef(propertyFilepath);
            GameEngine::FilePath defaultValue;
            property.getDefault(&defaultValue);
            if(defaultValue.path.empty())
            {
                if(defaultValue.fileType == GameEngine::FileType::IMAGE)
                    propertyFilepath->value_ = DataFile(DataFileType::Image);
            }
            else
                propertyFilepath->value_ = DataFile(defaultValue.path);
        }
            break;
        case GameEngine::PropertyTypes::ENUM: {
            PropertyEnumData *propertyEnum = new PropertyEnumData(property.name());
            propertyData = PropertyDataRef(propertyEnum);
            const auto &gamePropertyEnum = dynamic_cast<const GameEngine::PropertyEnumBase &>(property);
            property.getDefault(&propertyEnum->value_);
            propertyEnum->allowedValues_ = gamePropertyEnum.getAllowedValues();
        }
            break;
        default:
            if(property.type() == GameEngine::PropertyTypes::UNKNOWN)
                throw std::invalid_argument("property " + property.name() + " is an unknown type");
            else
                throw std::invalid_argument("property " + property.name() + " doesn't have valid deserializer");
            break;
    }

    propertyData->requrired_ = property.required();

    return propertyData;
}
