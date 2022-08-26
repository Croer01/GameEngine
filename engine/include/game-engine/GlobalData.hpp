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
        // This is a workarround to be able to build the map behind this
        // using msvc https://stackoverflow.com/a/59496238/6952678
        std::unique_ptr<geWritableData> dummy_;
        std::map<std::string, std::unique_ptr<geWritableData>> data_;

        geWritableData *getValue(const std::string &key) const;
        geWritableData *getOrCreateValue(const std::string &key);
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
