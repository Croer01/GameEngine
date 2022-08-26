//
// Created by adria on 11/03/2019.
//

#ifndef SPACEINVADERS_DATA_HPP
#define SPACEINVADERS_DATA_HPP

#include <game-engine/api.hpp>
#include <game-engine/geData.hpp>
#include <yaml-cpp/yaml.h>

namespace GameEngine{
namespace Internal{
    class PUBLICAPI Data : public virtual geData {
    protected:
        YAML::Node data_;
    public:
        Data();
        explicit Data(const YAML::Node &data);
        virtual ~Data() {};

        bool hasValue(const std::string &key) const override;
        std::string getString(const std::string &key) const override;
        int getInt(const std::string &key) const override;
        float getFloat(const std::string &key) const override;
        bool getBool(const std::string &key) const override;
        Vec2D getVec2D(const std::string &key) const override;
        std::vector<std::string> getArrayString(const std::string &key) const override;
        std::vector<Vec2D> getArrayVec2D(const std::string &key) const override;
        geColor getColor(const std::string &key) const override;

        YAML::Node yamlNode() const;
    };
}
}


#endif //SPACEINVADERS_DATA_HPP
