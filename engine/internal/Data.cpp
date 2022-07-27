//
// Created by adria on 11/03/2019.
//

#include <game-engine/internal/Data.hpp>
#include <game-engine/internal/yamlConverters.hpp>

namespace GameEngine{
namespace Internal{

    Data::Data() : Data(YAML::Node()){
    }

    Data::Data(const YAML::Node &data) {
        data_ = data;
    }

    std::string Data::getString(const std::string &key) const {
        return data_[key].as<std::string>("");
    }
    
    int Data::getInt(const std::string &key) const {
        return data_[key].as<int>(0);
    }
    
    float Data::getFloat(const std::string &key) const {
        return data_[key].as<float>(0.f);
    }
    
    bool Data::getBool(const std::string &key) const {
        return data_[key].as<bool>(false);
    }
    
    GameEngine::Vec2D Data::getVec2D(const std::string &key) const {
        return data_[key].as<Vec2D>(Vec2D(0.f,0.f));
    }
    
    std::vector<std::string> Data::getArrayString(const std::string &key) const {
        YAML::Node propertyNode = data_[key];
        std::vector<std::string> value;
        value.reserve(propertyNode.size());

        for (int i = 0; i < propertyNode.size(); ++i) {
            value.push_back(propertyNode[i].as<std::string>(""));
        }
        return value;
    }

    std::vector<Vec2D> Data::getArrayVec2D(const std::string &key) const {
        YAML::Node propertyNode = data_[key];
        std::vector<Vec2D> value;
        value.reserve(propertyNode.size());

        for (int i = 0; i < propertyNode.size(); ++i) {
            value.push_back(propertyNode[i].as<Vec2D>(Vec2D(0.f,0.f)));
        }
        return value;
    }

    geColor Data::getColor(const std::string &key) const {
        return data_[key].as<geColor>(geColor());
    }

    bool Data::hasValue(const std::string &key) const {
        return static_cast<bool>(data_[key]);
    }

    YAML::Node Data::yamlNode() const {
        return data_;
    }
}
}
