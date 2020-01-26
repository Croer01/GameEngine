//
// Created by adria on 11/10/2019.
//

#ifndef GAMEENGINEEDITOR_EDITOR_HPP
#define GAMEENGINEEDITOR_EDITOR_HPP


#include <memory>
#include <SDL2/SDL_video.h>
#include "ViewModels.hpp"
#include "CreateProjectEditor.hpp"
#include "GameComponentsProvider.hpp"
#include "ProjectPrototypeProvider.h"
#include "CreatePrototypeDialog.h"

class Editor
{
    SDL_Window* window_;
    std::unique_ptr<SceneData> sceneData_;
    ObjectDataRef objectSelected_;
    std::shared_ptr<ProjectData> project_;
    std::shared_ptr<CreateProjectEditor> createProjectEditor_;
    std::shared_ptr<CreatePrototypeDialog> createPrototypeDialog_;
    GameComponentsProvider gameComponentsProvider_;
    ProjectPrototypeProvider projectPrototypeProvider_;

    void generateMockData();
    void renderMainMenu();
    void renderSceneObjectNode(const ObjectDataRef &object, const std::string &id);
    void renderSceneInspector();
    void renderPrototypeList();
    void renderObjectInspector();
    void renderComponent(const ComponentDataRef &component);

    void setProject(const std::shared_ptr<ProjectData> &project);
    void createPrototype(const std::string &prototypeName);
    SceneData loadScene(const std::string &sceneFilePath);
    void updateWindowTitle();
public:
    explicit Editor(SDL_Window* window);
    void render();
    void loadProject();
    void saveProject();
};


#endif //GAMEENGINEEDITOR_EDITOR_HPP
