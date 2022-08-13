//
// Created by adria on 13/08/2022.
//

#ifndef GAMEENGINE_GLOBALDATA_HPP
#define GAMEENGINE_GLOBALDATA_HPP

#include <game-engine/geData.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace GameEngine {

    class PUBLICAPI GlobalData : public geWritableData {
        // This is a poor implementation but I only want to support a few
        // types, so in the future this may be to revisit.
        std::map<std::string, std::string> stringData_;
        std::map<std::string, int> intData_;
        std::map<std::string, float> floatData_;
        std::map<std::string, bool> boolData_;
        std::map<std::string, Vec2D> vec2dData_;
        std::map<std::string, std::vector<std::string>> stringArrayData_;
        std::map<std::string, std::vector<Vec2D>> vec2dArrayData_;
        std::map<std::string, geColor> colorData_;

        void checkValueExist(const std::string &key) const;
    public:

        bool hasValue(const std::string &key) const override;
        std::string getString(const std::string &key) const override;
        int getInt(const std::string &key) const override;
        float getFloat(const std::string &key) const override;
        bool getBool(const std::string &key) const override;
        Vec2D getVec2D(const std::string &key) const override;
        std::vector<std::string> getArrayString(const std::string &key) const override;
        std::vector<Vec2D> getArrayVec2D(const std::string &key) const override;
        geColor getColor(const std::string &key) const override;

        void setString(const std::string &key, const std::string &value) override;
        void setInt(const std::string &key, int value) override;
        void setFloat(const std::string &key, float value) override;
        void setBool(const std::string &key, bool value) override;
        void setVec2D(const std::string &key, const Vec2D &value) override;
        void setArrayString(const std::string &key, const std::vector<std::string> &value) override;
        void setArrayVec2D(const std::string &key, const std::vector<Vec2D> &value) override;
        void setColor(const std::string &key, const geColor &value) override;
    };
}
#endif //GAMEENGINE_GLOBALDATA_HPP
