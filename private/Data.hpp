//
// Created by adria on 11/03/2019.
//

#ifndef SPACEINVADERS_DATA_HPP
#define SPACEINVADERS_DATA_HPP

#include <game-engine/geData.hpp>
#include <yaml-cpp/yaml.h>

namespace GameEngine{
namespace Internal{
    class Data : public geData {
        YAML::Node data_;
    public:
        explicit Data(const YAML::Node &data);
        virtual ~Data() {};

        virtual bool hasValue(const std::string &key) const;
        virtual std::string getString(const std::string &key) const;
        virtual int getInt(const std::string &key) const;
        virtual float getFloat(const std::string &key) const;
        virtual bool getBool(const std::string &key) const;
        virtual Vec2D getVec2D(const std::string &key) const;
        virtual std::vector<std::string> getArrayString(const std::string &key) const;
        virtual std::vector<Vec2D> getArrayVec2D(const std::string &key) const;
        virtual geColor getColor(const std::string &key) const;

        YAML::Node yamlNode() const;
    };
}
}


#endif //SPACEINVADERS_DATA_HPP
