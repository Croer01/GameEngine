//
// Created by adria on 11/03/2019.
//

#include <game-engine/GlobalData.hpp>
#include <game-engine/internal/WritableData.hpp>
#include <stdexcept>

namespace GameEngine{

    constexpr const char *INTERNAL_VALUE_KEY = "internal_value";

    geWritableData *GlobalData::getValue(const std::string &key) const
    {
        auto it = data_.find(key);
        if(it == data_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second.get();
    }

    geWritableData *GlobalData::getOrCreateValue(const std::string &key)
    {
        auto it = data_.find(key);
        if(it == data_.end())
        {
            it = data_.emplace(key, std::move(std::unique_ptr<geWritableData>(new Internal::WritableData())))
                .first;
        }
        return it->second.get();
    }

    bool GlobalData::hasValue(const std::string &key) const {
        return data_.count(key);
    }

    std::string GlobalData::getString(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getString(INTERNAL_VALUE_KEY);
    }
    
    int GlobalData::getInt(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getInt(INTERNAL_VALUE_KEY);
    }
    
    float GlobalData::getFloat(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getFloat(INTERNAL_VALUE_KEY);
    }
    
    bool GlobalData::getBool(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getBool(INTERNAL_VALUE_KEY);
    }
    
    Vec2D GlobalData::getVec2D(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getVec2D(INTERNAL_VALUE_KEY);
    }
    
    std::vector<std::string> GlobalData::getArrayString(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getArrayString(INTERNAL_VALUE_KEY);
    }

    std::vector<Vec2D> GlobalData::getArrayVec2D(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getArrayVec2D(INTERNAL_VALUE_KEY);
    }

    geColor GlobalData::getColor(const std::string &key) const {
        geWritableData *data = getValue(key);
        return data->getColor(INTERNAL_VALUE_KEY);
    }

    void GlobalData::setString(const std::string &key, const std::string &value) {
        geWritableData *data = getOrCreateValue(key);
        data->setString(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setInt(const std::string &key, int value) {
        geWritableData *data = getOrCreateValue(key);
        data->setInt(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setFloat(const std::string &key, float value) {
        geWritableData *data = getOrCreateValue(key);
        data->setFloat(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setBool(const std::string &key, bool value) {
        geWritableData *data = getOrCreateValue(key);
        data->setBool(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setVec2D(const std::string &key, const Vec2D &value) {
        geWritableData *data = getOrCreateValue(key);
        data->setVec2D(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setArrayString(const std::string &key, const std::vector<std::string> &value) {
        geWritableData *data = getOrCreateValue(key);
        data->setArrayString(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setArrayVec2D(const std::string &key, const std::vector<Vec2D> &value) {
        geWritableData *data = getOrCreateValue(key);
        data->setArrayVec2D(INTERNAL_VALUE_KEY, value);
    }

    void GlobalData::setColor(const std::string &key, const geColor &value) {
        geWritableData *data = getOrCreateValue(key);
        data->setColor(INTERNAL_VALUE_KEY, value);
    }
}
