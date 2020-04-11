//
// Created by adria on 11/10/2019.
//

#ifndef GAMEENGINEEDITOR_EDITOR_HPP
#define GAMEENGINEEDITOR_EDITOR_HPP


#include <memory>
#include <future>
#include <SDL2/SDL_video.h>
#include "ViewModels.hpp"
#include "CreateProjectEditor.hpp"
#include "GameComponentsProvider.hpp"
#include "ProjectFileDataProvider.h"
#include "dialogs/CreatePrototypeDialog.h"
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
    SceneDataRef sceneData_;
    std::unique_ptr<ProjectDirectory> projectDirectory_;
    std::unique_ptr<TargetObject> objectSelected_;
    std::shared_ptr<ProjectData> project_;
    std::shared_ptr<CreateProjectEditor> createProjectEditor_;
    std::shared_ptr<CreatePrototypeDialog> createPrototypeDialog_;
    std::shared_ptr<DeleteFileDialog> deleteFileDialog_;
    std::shared_ptr<SaveAllDialog> saveAllDialog_;
    std::shared_ptr<ErrorDialog> errorDialog_;
    GameComponentsProvider gameComponentsProvider_;
    ProjectFileDataProvider projectFileDataProvider_;
    std::future<int> gameThread_;

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
    void deleteFile(const boost::filesystem::path &filePath);
    void loadScene(const std::string &sceneFilePath);
    void updateWindowTitle();

    static void setPosToColumnCenter(float width = 0);
public:
    explicit Editor(SDL_Window* window);
    void render();
    void loadProject();
    void saveProject();
};


#endif //GAMEENGINEEDITOR_EDITOR_HPP
