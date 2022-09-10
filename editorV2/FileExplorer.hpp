//
// Created by adria on 28/01/2021.
//

#ifndef GAMEEDITOR_FILEEXPLORER_HPP
#define GAMEEDITOR_FILEEXPLORER_HPP


#include <memory>
#include <game-engine/internal/Environment.hpp>
#include "DataDirectory.hpp"

enum class FileExplorerEvent
{
    SelectObject
};

class FileExplorer : public GameEngine::Subject<FileExplorerEvent, const GameEngine::geGameObjectRef &>
{
    DataDirectoryRef root_;
    std::shared_ptr<GameEngine::Internal::Environment> env_;

    void renderFileListInternal(const DataDirectoryRef &dir);
    void recursiveFilesRegister(const boost::filesystem::path &directoryPath);
public:
    explicit FileExplorer(const std::shared_ptr<GameEngine::Internal::Environment> &env);
    void setRoot(const boost::filesystem::path &directory);
    void update();

};


#endif //GAMEEDITOR_FILEEXPLORER_HPP
