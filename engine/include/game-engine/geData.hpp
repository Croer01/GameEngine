//
// Created by adria on 11/03/2019.
//

#ifndef SPACEINVADERS_GEDATA_HPP
#define SPACEINVADERS_GEDATA_HPP

#include <game-engine/api.hpp>
#include <string>
#include <vector>
#include <memory>
#include <game-engine/FileType.hpp>

namespace GameEngine {
    class geData;

    typedef std::shared_ptr<geData> geDataRef;

    class PUBLICAPI geData {
    public:
        virtual ~geData(){};

        virtual bool hasValue(const std::string &key) const = 0;
        virtual std::string getString(const std::string &key) const = 0;
        virtual int getInt(const std::string &key) const = 0;
        virtual float getFloat(const std::string &key) const = 0;
        virtual bool getBool(const std::string &key) const = 0;
        virtual Vec2D getVec2D(const std::string &key) const = 0;
        virtual std::vector<std::string> getArrayString(const std::string &key) const = 0;
        virtual std::vector<Vec2D> getArrayVec2D(const std::string &key) const = 0;
        virtual geColor getColor(const std::string &key) const = 0;
    };
}
#endif //SPACEINVADERS_GEDATA_HPP
