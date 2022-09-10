//
// Created by adria on 11/03/2019.
//

#ifndef GAMEENGINE_WRITABLEDATA_HPP
#define GAMEENGINE_WRITABLEDATA_HPP

#include <game-engine/api.hpp>
#include <game-engine/internal/Data.hpp>
#include <yaml-cpp/yaml.h>

namespace GameEngine{
namespace Internal{
    class PUBLICAPI WritableData : public Data, public geWritableData {
    public:
        WritableData();
        virtual ~WritableData() {};

        void setString(const std::string &key, const std::string &value) override;
        void setInt(const std::string &key, int value) override;
        void setFloat(const std::string &key, float value) override;
        void setBool(const std::string &key, bool value) override;
        void setVec2D(const std::string &key, const Vec2D &value) override;
        void setArrayString(const std::string &key, const std::vector<std::string> &value) override;
        void setArrayVec2D(const std::string &key, const std::vector<Vec2D> &value) override;
        void setColor(const std::string &key, const geColor &value) override;
        void setChild(const std::string &key, const WritableData &data);
    };
}
}


#endif //GAMEENGINE_WRITABLEDATA_HPP
