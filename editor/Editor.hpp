//
// Created by adria on 11/10/2019.
//

#ifndef GAMEENGINEEDITOR_EDITOR_HPP
#define GAMEENGINEEDITOR_EDITOR_HPP


#include <memory>
#include <future>
#include <SDL2/SDL_video.h>
#include <game-engine/geGame.hpp>
#include "ViewModels.hpp"
#include "CreateProjectEditor.hpp"
#include "GameComponentsProvider.hpp"
#include "ProjectFileDataProvider.h"
#include "dialogs/CreatePrototypeDialog.h"
#include "dialogs/DeleteFileDialog.h"
#include "dialogs/SaveAllDialog.h"
#include "dialogs/ErrorDialog.h"
#include "dialogs/CreateSceneDialog.h"

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
    std::unique_ptr<GameEngine::geRendererLock> renderMutex_;
    SceneDataRef sceneData_;
    std::unique_ptr<ProjectDirectory> projectDirectory_;
    std::unique_ptr<TargetObject> objectSelected_;
    std::shared_ptr<ProjectData> project_;
    std::shared_ptr<CreateProjectEditor> createProjectEditor_;
    std::shared_ptr<CreatePrototypeDialog> createPrototypeDialog_;
    std::shared_ptr<CreateSceneDialog> createSceneDialog_;
    std::shared_ptr<DeleteFileDialog> deleteFileDialog_;
    std::shared_ptr<SaveAllDialog> saveAllDialog_;
    std::shared_ptr<ErrorDialog> errorDialog_;
    GameComponentsProvider gameComponentsProvider_;
    ProjectFileDataProvider projectFileDataProvider_;
    std::future<int> gameThread_;
    GameEngine::geGameRef game_;

    void renderMainMenu();
    bool renderSceneObjectNode(const PrototypeReferenceRef &object, const std::string &id);
    void renderSceneInspector();
    void renderSceneViewer();
    void renderPrototypeList();
    void renderPrototypeInspector();
    bool renderObjectNode(const ObjectDataRef &object);
    void renderCentralRegion();
    void renderGuiInspector();
    void renderPhysicsInspector();
    bool renderComponent(const ComponentDataRef &component);
    bool renderFileSelector(const std::string &label, const std::string &currentFile, DataFileType typeSupported, DataFile *result);

    void setProject(const std::shared_ptr<ProjectData> &project);
    void createPrototype(const std::string &prototypeName);
    void createScene(const std::string &sceneName);
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
    GameEngine::geGameRef game() const;
};


#endif //GAMEENGINEEDITOR_EDITOR_HPP
