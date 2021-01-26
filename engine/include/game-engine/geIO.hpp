//
// Created by adria on 26/03/2019.
//

#ifndef GAMEENGINE_GEIO_HPP
#define GAMEENGINE_GEIO_HPP

#include <game-engine/api.hpp>
#include <game-engine/geData.hpp>

namespace GameEngine {
    class PUBLICAPI geIO {
    public:
        geDataRef load(const std::string &filename);
        void save(const std::string &filename, const geDataRef &data);
    };

    class PUBLICAPI geDataWriter {
        geDataRef data_;
    public:
        explicit geDataWriter(const geDataRef &data) : data_(data){};
        virtual ~geDataWriter(){};
        virtual void writeString(const std::string &key, const std::string &value);
        virtual void writeInt(const std::string &key, int value);
        virtual void writeFloat(const std::string &key, float value);
        virtual void writeBool(const std::string &key, bool value);
        virtual void writeVec2D(const std::string &key, const Vec2D &value);
        virtual void writeArrayString(const std::string &key, const std::vector<std::string> &value);
    };
}

#endif //GAMEENGINE_GEIO_HPP
