//
// Created by adria on 11/10/2019.
//

#ifndef GAMEENGINEEDITOR_EDITOR_HPP
#define GAMEENGINEEDITOR_EDITOR_HPP


#include <memory>
#include <future>
#include <SDL2/SDL_video.h>
#include <game-engine/Game.hpp>
#include "ViewModels.hpp"
#include "CreateProjectEditor.hpp"
#include "GameComponentsProvider.hpp"
#include "ProjectFileDataProvider.h"
#include "dialogs/CreateDataFileDialog.h"
#include "dialogs/DeleteFileDialog.h"
#include "dialogs/SaveAllDialog.h"
#include "dialogs/ErrorDialog.h"

class Editor
{
    struct TargetObject{
        ObjectDataRef data;
        DataFile sourceFile;

        TargetObject() : sourceFile("")
        {}
    };

    SDL_Window* window_;
    SDL_GLContext glContext_;
    SDL_GLContext gameGlContext_;
    std::unique_lock<std::mutex> renderMutex_;
    SceneDataRef sceneData_;
    std::unique_ptr<ProjectDirectory> projectDirectory_;
    std::unique_ptr<TargetObject> objectSelected_;
    std::shared_ptr<ProjectData> project_;
    std::shared_ptr<CreateProjectEditor> createProjectEditor_;
    std::shared_ptr<CreateDataFileDialog> createDataFileDialog_;
    std::shared_ptr<DeleteFileDialog> deleteFileDialog_;
    std::shared_ptr<SaveAllDialog> saveAllDialog_;
    std::shared_ptr<ErrorDialog> errorDialog_;
    GameComponentsProvider gameComponentsProvider_;
    ProjectFileDataProvider projectFileDataProvider_;
    std::future<int> gameThread_;
    GameEngine::GameRef game_;

    void renderMainMenu();
    bool renderSceneObjectNode(const PrototypeReferenceRef &object, const std::string &id, bool &deleteObject);
    void renderSceneInspector();
    void renderSceneViewer();
    void renderDataDirectoryExplorer();
    void renderPrototypeListInternal(const DataDirectoryRef &dir);
    void renderPrototypeInspector();
    bool renderObjectNode(const ObjectDataRef &object);
    void renderCentralRegion();
    void renderGuiInspector();
    void renderPhysicsInspector();
    bool renderComponent(const ComponentDataRef &component, int index, int length, int *moveComponent, int *removeComponent);
    bool renderFileSelector(const std::string &label, const std::string &currentFile, DataFileType typeSupported, DataFile *result);

    void setProject(const std::shared_ptr<ProjectData> &project);
    void createPrototype(const boost::filesystem::path &prototypeFilepath);
    void createScene(const boost::filesystem::path &sceneFilepath);
    void deleteFile(const boost::filesystem::path &filePath);
    void loadScene(const std::string &sceneFilePath);
    void updateWindowTitle();

    static void setPosToColumnCenter(float width = 0);
public:
    Editor(SDL_Window* window, SDL_GLContext glContext);
    void render();
    void loadProject();
    void saveProject();
    void makeCurrentContext();
    void releaseCurrentContext();
    void shutdown();
    GameEngine::GameRef game() const;
    void starGame(bool update);
    void reloadGameStaticScene();
};


#endif //GAMEENGINEEDITOR_EDITOR_HPP
