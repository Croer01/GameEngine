//
// Created by adria on 09/02/2021.
//

#ifndef GAMEENGINE_GAMEOBJECTEDITOR_HPP
#define GAMEENGINE_GAMEOBJECTEDITOR_HPP


#include <memory>
#include <game-engine/internal/GameObject.hpp>
#include "DataFile.hpp"
#include "DataDirectory.hpp"
#include "FileExplorer.hpp"

class GameObjectEditor : public GameEngine::Observer<FileExplorerEvent , const GameEngine::geGameObjectRef &>
{
    std::shared_ptr<GameEngine::Internal::GameObject> target_;
    DataDirectoryRef root_;

    bool renderPropertyByType(const std::shared_ptr<GameEngine::PropertyBase> &property);
    bool renderFileSelector(const std::string &label, const std::string &currentFile, DataFileType typeSupported, DataFileRef &result);
public:
    virtual void onEvent(const GameEngine::Subject<FileExplorerEvent, const GameEngine::geGameObjectRef &> &target,
                         FileExplorerEvent, const GameEngine::geGameObjectRef &goTarget);
public:
    void setRoot(const boost::filesystem::path &directory);
    void setTarget(const GameEngine::geGameObjectRef &target);

    void update();
};


#endif //GAMEENGINE_GAMEOBJECTEDITOR_HPP
