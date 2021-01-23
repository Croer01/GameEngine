//
// Created by adria on 20/09/2020.
//

#include "WritableData.hpp"
#include "yamlConverters.hpp"

GameEngine::Internal::WritableData::WritableData() : Data(YAML::Node())
{
}

void GameEngine::Internal::WritableData::setString(const std::string &key, std::string value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setInt(const std::string &key, int value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setFloat(const std::string &key, float value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setBool(const std::string &key, bool value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setVec2D(const std::string &key, GameEngine::Vec2D value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setArrayString(const std::string &key, std::vector<std::string> value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setArrayVec2D(const std::string &key, std::vector<Vec2D> value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setColor(const std::string &key, GameEngine::geColor value)
{
    data_[key] = value;
}

void GameEngine::Internal::WritableData::setChild(const std::string &key,const GameEngine::Internal::WritableData &data)
{
    data_[key] = data.data_;
}
