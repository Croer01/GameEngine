//
// Created by adria on 07/02/2020.
//

#include "game-engine/FilePath.h"

namespace GameEngine {
FilePath::FilePath(const std::string &path) : path(path), fileType(FileType::OTHER)
{}

FilePath::FilePath(const std::string &path, const FileType &fileType) : path(path), fileType(fileType)
{}

FilePath::FilePath()
        : path(""), fileType(FileType::OTHER)
{
}
}
