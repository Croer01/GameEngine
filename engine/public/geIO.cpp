//
// Created by adria on 26/03/2019.
//

#include <game-engine/internal/Data.hpp>
#include <game-engine/geIO.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <game-engine/internal/yamlConverters.hpp>

namespace GameEngine {

    geDataRef geIO::load(const std::string &filename) {
        std::ifstream infile(filename);
        YAML::Node node;

        if(infile.good()) {
            node = YAML::Load(infile);
        }

        return std::make_shared<Internal::Data>(node);
    }

    void GameEngine::geIO::save(const std::string &filename, const geDataRef &data) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data);

        const YAML::Node &node = internalData->yamlNode();
        std::ofstream fout(filename);
        fout << node;
    }

    void geDataWriter::writeString(const std::string &key, const std::string &value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }

    void geDataWriter::writeInt(const std::string &key, int value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }

    void geDataWriter::writeFloat(const std::string &key, float value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }

    void geDataWriter::writeBool(const std::string &key, bool value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }

    void geDataWriter::writeVec2D(const std::string &key, const Vec2D &value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }

    void geDataWriter::writeArrayString(const std::string &key, const std::vector<std::string> &value) {
        const auto &internalData = std::dynamic_pointer_cast<Internal::Data>(data_);
        internalData->yamlNode()[key] = value;
    }
};
