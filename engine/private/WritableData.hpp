//
// Created by adria on 11/03/2019.
//

#ifndef GAMEENGINE_WRITABLEDATA_HPP
#define GAMEENGINE_WRITABLEDATA_HPP

#include "Data.hpp"
#include <yaml-cpp/yaml.h>

namespace GameEngine{
namespace Internal{
    class WritableData : public Data {
    public:
        WritableData();
        virtual ~WritableData() {};

        void setString(const std::string &key, std::string value);
        void setInt(const std::string &key, int value);
        void setFloat(const std::string &key, float value);
        void setBool(const std::string &key, bool value);
        void setVec2D(const std::string &key, Vec2D value);
        void setArrayString(const std::string &key, std::vector<std::string> value);
        void setArrayVec2D(const std::string &key, std::vector<Vec2D> value);
        void setColor(const std::string &key, geColor value);
        void setChild(const std::string &key, const WritableData &data);
    };
}
}


#endif //GAMEENGINE_WRITABLEDATA_HPP
