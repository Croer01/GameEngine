//
// Created by adria on 11/03/2019.
//

#include <game-engine/GlobalData.hpp>
#include <stdexcept>

namespace GameEngine{

    void GlobalData::checkValueExist(const std::string &key) const
    {
        if(hasValue(key))
            throw std::runtime_error("GlobalData value " + key + " already exist");
    }

    bool GlobalData::hasValue(const std::string &key) const {
        return stringData_.count(key) ||
            intData_.count(key) ||
            floatData_.count(key) ||
            boolData_.count(key) ||
            vec2dData_.count(key) ||
            stringArrayData_.count(key) ||
            vec2dArrayData_.count(key) ||
            colorData_.count(key);
    }

    std::string GlobalData::getString(const std::string &key) const {
        auto it = stringData_.find(key);
        if(it == stringData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }
    
    int GlobalData::getInt(const std::string &key) const {
        auto it = intData_.find(key);
        if(it == intData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }
    
    float GlobalData::getFloat(const std::string &key) const {
        auto it = floatData_.find(key);
        if(it == floatData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }
    
    bool GlobalData::getBool(const std::string &key) const {
        auto it = boolData_.find(key);
        if(it == boolData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }
    
    Vec2D GlobalData::getVec2D(const std::string &key) const {
        auto it = vec2dData_.find(key);
        if(it == vec2dData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }
    
    std::vector<std::string> GlobalData::getArrayString(const std::string &key) const {
        auto it = stringArrayData_.find(key);
        if(it == stringArrayData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }

    std::vector<Vec2D> GlobalData::getArrayVec2D(const std::string &key) const {
        auto it = vec2dArrayData_.find(key);
        if(it == vec2dArrayData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }

    geColor GlobalData::getColor(const std::string &key) const {
        auto it = colorData_.find(key);
        if(it == colorData_.end())
            throw std::runtime_error("GlobalData value " + key + " doesn't exist");
        return it->second;
    }

    void GlobalData::setString(const std::string &key, const std::string &value) {
        checkValueExist(key);
        stringData_[key] = value;
    }
    void GlobalData::setInt(const std::string &key, int value) {
        checkValueExist(key);
        intData_[key] = value;
    }
    void GlobalData::setFloat(const std::string &key, float value) {
        checkValueExist(key);
        floatData_[key] = value;
    }
    void GlobalData::setBool(const std::string &key, bool value) {
        checkValueExist(key);
        boolData_[key] = value;
    }
    void GlobalData::setVec2D(const std::string &key, const Vec2D &value) {
        checkValueExist(key);
        vec2dData_[key] = value;
    }
    void GlobalData::setArrayString(const std::string &key, const std::vector<std::string> &value) {
        checkValueExist(key);
        stringArrayData_[key] = value;
    }
    void GlobalData::setArrayVec2D(const std::string &key, const std::vector<Vec2D> &value) {
        checkValueExist(key);
        vec2dArrayData_[key] = value;
    }
    void GlobalData::setColor(const std::string &key, const geColor &value) {
        checkValueExist(key);
        colorData_[key] = value;
    }
}
