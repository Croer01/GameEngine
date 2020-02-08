//
// Created by adria on 07/02/2020.
//

#ifndef GAMEDEVWARS_FILEPATH_H
#define GAMEDEVWARS_FILEPATH_H


#include <game-engine/api.hpp>
#include <string>

namespace GameEngine {
    enum class FileType
    {
        OTHER,
        PROTOTYPE,
        SCENE,
        IMAGE
    };

    struct PUBLICAPI FilePath
    {
        std::string path;
        FileType fileType;

        FilePath();
        explicit FilePath(const std::string &path);
        FilePath(const std::string &path, const FileType &fileType);
    };

}

#endif //GAMEDEVWARS_FILEPATH_H
