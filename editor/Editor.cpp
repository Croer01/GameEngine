//
// Created by adria on 11/10/2019.
//

#include <imgui.h>
#include "imgui_backend/imgui_stdlib.h"
#include <sstream>
#include "Editor.hpp"
#include <functional>
#include <boost/filesystem/path.hpp>
#include <tinyfiledialogs.h>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <game-engine/geEnvironment.hpp>

namespace fs = boost::filesystem;

Editor::Editor(SDL_Window *window):
    window_(window)
{
    ImGui::GetStyle().WindowRounding = 0.f;
    ImGui::GetStyle().FrameRounding = 3.f;

    auto createProjectCallback = std::bind(&Editor::setProject, this, std::placeholders::_1);
    createProjectEditor_= std::make_shared<CreateProjectEditor>(createProjectCallback);
    
    auto createPrototypeCallback = std::bind(&Editor::createPrototype, this, std::placeholders::_1);
    createPrototypeDialog_ = std::make_shared<CreatePrototypeDialog>(createPrototypeCallback);

    auto deleteFileCallback = std::bind(&Editor::deleteFile, this, std::placeholders::_1);
    deleteFileDialog_ = std::make_shared<DeleteFileDialog>(deleteFileCallback);

    saveAllDialog_ = std::make_shared<SaveAllDialog>([=](const fs::path &result){
        saveProject();
        if(!result.empty())
            loadScene(result.string());
    });
    errorDialog_ = std::make_shared<ErrorDialog>();
}

void Editor::render()
{
    renderMainMenu();

    createProjectEditor_->Render();
    createPrototypeDialog_->Render();
    deleteFileDialog_->Render();
    saveAllDialog_->Render();
    errorDialog_->Render();

    if(project_)
    {
        bool prevDirty = project_->dirty_ || projectDirectory_->hasEditedFiles();

        ImGui::SetNextWindowPosCenter();
        ImGui::Begin("no dirty");
        if(ImGui::Button("no dirty"))
            project_->dirty_ = false;
        ImGui::End();

        renderSceneInspector();
        renderPrototypeList();
        renderObjectInspector();

        if(prevDirty != project_->dirty_)
            updateWindowTitle();
    }
    else
    {
        ImGui::SetNextWindowPosCenter();
        ImGui::Begin("Welcome");
        if(ImGui::Button("Create Project..."))
            createProjectEditor_->open();

        if(ImGui::Button("Load Project..."))
            loadProject();
        ImGui::End();
    }
}

bool Editor::renderSceneObjectNode(const PrototypeReferenceRef &object, const std::string &id)
{
    bool edited = false;

    ImGui::PushID(("ObjectNode" + id).c_str());
    if (ImGui::TreeNodeEx(id.c_str(), ImGuiTreeNodeFlags_OpenOnArrow, "%s", object->name_.c_str()))
    {
        ImGui::InputText("Name",&object->name_);
        if(ImGui::IsItemEdited())
            edited = true;

        ImGui::InputText("prototype",&object->prototype_);
        if(ImGui::IsItemEdited())
            edited = true;

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if(ImGui::DragFloat2("position", object->position_.xy.data(),.1f))
                edited = true;
            if(ImGui::DragFloat2("size", object->scale_.xy.data(), .1f, 1.f, std::numeric_limits<float>::max()))
                edited = true;
            if(ImGui::DragFloat2("rotation", object->rotation_.xy.data(),.1f))
                edited = true;
        }

        ImGui::TreePop();
            ImGui::Separator();
    }
    ImGui::PopID();

    return edited;
}

void Editor::renderPrototypeList()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    size.y /= 2.f;
    ImGui::SetNextWindowPos(ImVec2(0,size.y + 20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Prototypes",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(ImGui::Button("Create..."))
    {
        const std::string &defaultValue =
                "Prototype" + std::to_string(projectDirectory_->getFiles().size());
        createPrototypeDialog_->open(defaultValue);
    }

    ImGui::Separator();
    ImGuiTreeNodeFlags PrototypesNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    int i = 0;
    for (const auto &dataFile : projectDirectory_->getFiles())
    {
        const fs::path& filepath = dataFile.getFilePath();
        ImGui::PushID(i++);
        const boost::filesystem::path &relativeFilePath = fs::relative(filepath, project_->dataPath_);
        ImGui::TreeNodeEx(relativeFilePath.string().c_str(), PrototypesNodeFlags);
        if(ImGui::IsItemClicked())
        {
            switch (dataFile.getType())
            {
                case DataFileType::Prototype :
                    try {
                        auto object = new TargetObject();
                        object->data = projectPrototypeProvider_.getPrototype(dataFile);
                        object->sourceFile = DataFile(filepath);

                        objectSelected_.reset(object);
                    }
                    catch (const std::exception &e)
                    {
                        errorDialog_->open(e.what());
                    }
                    break;
                case DataFileType::Scene:
                    if(projectDirectory_->hasEditedFiles())
                    {
                        saveAllDialog_->setFilesToSave(projectDirectory_->getEditedFiles());
                        saveAllDialog_->open(filepath);
                    }
                    else
                    {
                        loadScene(filepath.string());
                    }
                    break;
            }
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 20);
        if(ImGui::Button("X"))
            deleteFileDialog_->open(relativeFilePath);
        ImGui::PopID();
    }

    ImGui::End();
}

void Editor::createPrototype(const std::string &prototypeName)
{
    fs::path prototypePath(this->project_->dataPath_);

    std::stringstream ss;
    ss << prototypeName << ".prototype";
    prototypePath.append(ss.str());

    ObjectData newPrototype;
    newPrototype.name_ = prototypePath.stem().string();

    YAML::Node prototypeNode;
    prototypeNode = newPrototype;
    std::ofstream prototypeFile;
    prototypeFile.open(prototypePath.string());
    prototypeFile << prototypeNode << std::endl;
    prototypeFile.close();
    projectDirectory_->addFile(prototypePath);
}

void Editor::renderSceneInspector()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    size.y /= 2.f;
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Scene Inspector",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(ImGui::InputText("Name", &sceneData_->name_))
    {
        updateWindowTitle();
    }
    ImGui::Separator();
    if(ImGui::Button("New Object"))
    {
        PrototypeReferenceRef newObject = std::make_shared<PrototypeReference>();
        std::stringstream ss;
        ss << "Object" << sceneData_->objects_.size();
        newObject->name_ = ss.str();
        sceneData_->objects_.push_back(newObject);
    }

    for (auto i = 0; i < sceneData_->objects_.size(); i++)
    {
        if(renderSceneObjectNode(sceneData_->objects_[i], std::to_string(i)))
        {
            project_->dirty_ = true;
            projectDirectory_->markEdited(DataFile(sceneData_->filePath_));
        }
    }

    ImGui::End();
}

void Editor::renderObjectInspector(){
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    ImVec2 pos(0.f,20.f);
    pos.x = ImGui::GetIO().DisplaySize.x * 0.75f;
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);

    std::string objectLabel = objectSelected_? objectSelected_->data->name_ : "Empty";

    ImGui::Begin((objectLabel + " - Properties Inspector###PropertyInspector").c_str(), 0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(objectSelected_)
    {
        ObjectDataRef objectDataSelected = objectSelected_->data;
        bool edited = false;
        ImGui::InputText("Name",&objectDataSelected->name_);
        if(ImGui::IsItemEdited())
            edited = true;

        if(ImGui::Button("New Child"))
        {
            ObjectDataRef newChild = std::make_shared<ObjectData>();
            std::stringstream ss;
            ss << "Child" << objectDataSelected->children_.size();
            newChild->name_ = ss.str();
            objectDataSelected->children_.push_back(newChild);
            edited = true;
        }

        ImGui::Separator();
        static std::string item_current = gameComponentsProvider_.getRegisteredPropertiesIds()[0];
        if (ImGui::BeginCombo("Component", item_current.c_str())) // The second parameter is the label previewed before opening the combo.
        {
            auto components = gameComponentsProvider_.getRegisteredPropertiesIds();
            for (std::string & component : components)
            {
                bool is_selected = item_current == component;
                if (ImGui::Selectable(component.c_str(), is_selected))
                    item_current = component;
                // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if(ImGui::Button("Add Component..."))
        {
            assert(objectDataSelected);
            ComponentDataRef newComponent = std::make_shared<ComponentData>();
            newComponent->name_ = item_current;
            newComponent->properties_ = gameComponentsProvider_.getPropertiesMetadata(item_current);

            objectDataSelected->components_.push_back(newComponent);
            edited = true;
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if(ImGui::DragFloat2("position", objectDataSelected->position_.xy.data(),.1f))
                edited = true;
            if(ImGui::DragFloat2("size", objectDataSelected->scale_.xy.data(), .1f, 1.f, std::numeric_limits<float>::max()))
                edited = true;
            if(ImGui::DragFloat2("rotation", objectDataSelected->rotation_.xy.data(),.1f))
                edited = true;
        }

        ImGui::PushID(objectDataSelected->name_.c_str());
        for (const auto &component : objectDataSelected->components_)
        {
            if(renderComponent(component))
                edited = true;
        }
        ImGui::PopID();

        //remove components safety
        auto it = std::remove_if( objectDataSelected->components_.begin(),
                objectDataSelected->components_.end(),
                [](const ComponentDataRef &component){ return component->markToRemove_; } );
        if(it != objectDataSelected->components_.end()){
            objectDataSelected->components_.erase( it, objectDataSelected->components_.end() );
            edited= true;
        }

        if(edited) {
            project_->dirty_ = true;
            projectDirectory_->markEdited(objectSelected_->sourceFile);
        }
    }
    ImGui::End();
}

bool Editor::renderComponent(const ComponentDataRef &component)
{
    bool edited = false;

    ImGui::PushID(component->name_.c_str());
    bool opened = ImGui::CollapsingHeader(component->name_.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap);
    ImGui::SameLine(ImGui::GetWindowWidth() - 20);
    if(ImGui::Button("x"))
    {
        component->markToRemove_ = true;
    }

    if (opened)
    {
        for (const auto &property : component->properties_)
        {
            ImGui::PushID(property->name_.c_str());
            switch (property->type_)
            {
                case PropertyDataType::INT:
                {
                    auto propertyInt = std::dynamic_pointer_cast<PropertyIntData>(property);
                    ImGui::DragInt(property->name_.c_str(), &propertyInt->value_, .1f);
                    if(ImGui::IsItemEdited())
                        edited = true;
                }
                    break;
                case PropertyDataType::FLOAT:
                {
                    auto propertyFloat = std::dynamic_pointer_cast<PropertyFloatData>(property);
                    ImGui::DragFloat(property->name_.c_str(), &propertyFloat->value_, .1f);
                    if(ImGui::IsItemEdited())
                        edited = true;
                }
                    break;
                case PropertyDataType::STRING:
                {
                    auto propertyString = std::dynamic_pointer_cast<PropertyStringData>(property);
                    ImGui::InputText(propertyString->name_.c_str(), &propertyString->value_);
                    if(ImGui::IsItemEdited())
                        edited = true;
                }
                    break;
                case PropertyDataType::BOOL:
                {
                    auto propertyBool = std::dynamic_pointer_cast<PropertyBoolData>(property);
                    ImGui::Checkbox(property->name_.c_str(), &propertyBool->value_);
                    if(ImGui::IsItemEdited())
                        edited = true;
                }
                    break;
                case PropertyDataType::VEC2D:
                {
                    auto propertyVec2D = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    if(ImGui::DragFloat2(property->name_.c_str(), propertyVec2D->value_.xy.data()))
                        edited = true;
                }
                    break;
                case PropertyDataType::COLOR:
                {
                    auto propertyColor = std::dynamic_pointer_cast<PropertyColorData>(property);
                    if(ImGui::ColorEdit3(property->name_.c_str(), propertyColor->value_.rgb.data()))
                        edited = true;
                }
                    break;
                case PropertyDataType::ARRAY_STRING:
                {
                    auto propertyStringArray = std::dynamic_pointer_cast<PropertyStringArrayData>(property);

                    ImGui::Text("%s", propertyStringArray->name_.c_str());
                    ImGui::SameLine();
                    if(ImGui::Button("+"))
                    {
                        propertyStringArray->value_.emplace_back("");
                        edited = true;
                    }

                    auto &values = propertyStringArray->value_;
                    for (int i = 0; i < values.size(); ++i)
                    {
                        ImGui::PushID(i);
                        ImGui::InputText("", &values[i]);
                        if(ImGui::IsItemEdited())
                            edited = true;
                        ImGui::PopID();
                    }
                }
                    break;
                case PropertyDataType::ARRAY_VEC2D:
                {
                    auto propertyVec2DArray = std::dynamic_pointer_cast<PropertyVec2DArrayData>(property);

                    ImGui::Text("%s", propertyVec2DArray->name_.c_str());
                    ImGui::SameLine();
                    if(ImGui::Button("+"))
                    {
                        propertyVec2DArray->value_.emplace_back();
                        edited = true;
                    }

                    auto &values = propertyVec2DArray->value_;
                    for (int i = 0; i < values.size(); ++i)
                    {
                        ImGui::PushID(i);
                        Vector2DData &value = values[i];
                        if(ImGui::DragFloat2("", value.xy.data()))
                            edited = true;
                        ImGui::PopID();
                    }
                }
                    break;
                case PropertyDataType::FILEPATH:
                {
                    auto propertyFilePath = std::dynamic_pointer_cast<PropertyFilePathData>(property);
                    DataFile fileSelected;
                    if(renderFileSelector(propertyFilePath->name_.c_str(), propertyFilePath->value_, &fileSelected))
                    {
                        propertyFilePath->value_ = fileSelected;
                        edited = true;
                    }
                }
                    break;
                case PropertyDataType::ENUM:
                {
                    auto propertyEnum = std::dynamic_pointer_cast<PropertyEnumData>(property);

                    std::string newValue;
                    if (ImGui::BeginCombo(propertyEnum->name_.c_str(), propertyEnum->value_.c_str()))
                    {
                        for(const auto &item : propertyEnum->allowedValues_)
                        {
                            bool is_selected = (propertyEnum->value_ == item);
                            if (ImGui::Selectable(item.c_str(), is_selected))
                            {
                                newValue = item;
                            }
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    if(!newValue.empty() && propertyEnum->value_ != newValue)
                    {
                        propertyEnum->value_ = newValue;
                        edited = true;
                    }
                }
                    break;
            }
            ImGui::PopID();
        }
    }
    ImGui::PopID();

    return edited;
}

void Editor::renderMainMenu()
{
    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Project", NULL, false, false);
            if (ImGui::MenuItem("Create..."))
            {
                createProjectEditor_->open();
            }

            if (ImGui::MenuItem("Open..."))
            {
                loadProject();
            }

            if (ImGui::MenuItem("Save all", NULL, false, project_ && project_->dirty_))
            {
                saveProject();
            }
            ImGui::Separator();

            std::string sceneName = "Scene - ";
            if(sceneData_)
                sceneName += sceneData_->name_;
            else
                sceneName += "[Empty]";

            ImGui::MenuItem(sceneName.c_str(), NULL, false, false);
            if (ImGui::MenuItem("Create...", NULL, false, (bool)sceneData_))
            {
                sceneData_.reset(new SceneData());
                objectSelected_.reset();
                updateWindowTitle();
            }

            if (ImGui::MenuItem("Load...", NULL, false, (bool)sceneData_))
            {
                char const * filter[1] = {"*.scene"};
                char const *sceneFile = tinyfd_openFileDialog("Load Scene", "", 1, filter, "Scene file (*.scene)", 0);
                if (sceneFile)
                {
                    loadScene(sceneFile);
                }
            }

            bool save = false;
            bool saveAs = false;
            if (ImGui::MenuItem("Save", NULL, false, (bool)sceneData_))
            {
                if(sceneData_->filePath_.empty())
                    saveAs = true;
                else
                    save = true;
            }

            if (ImGui::MenuItem("Save as...", NULL, false, (bool)sceneData_))
            {
                saveAs = true;
            }

            if(saveAs)
            {
                char const *filter[1] = {"*.scene"};
                char const *sceneFile = tinyfd_saveFileDialog("Save Scene", "", 1, filter, "Scene file (*.scene)");
                if (sceneFile)
                {
                    sceneData_->filePath_ = fs::path(sceneFile).replace_extension(".scene").string();
                    save = true;
                }
            }

            if(save)
            {
                YAML::Node sceneNode;
                sceneNode = *sceneData_;
                std::ofstream sceneFile;
                sceneFile.open(sceneData_->filePath_);
                sceneFile << sceneNode << std::endl;
                sceneFile.close();

                //TODO: review this block code "if(save){ ... }"
                project_->currentScenePath_ = sceneData_->filePath_;
                YAML::Node projectNode;
                projectNode = *project_;
                std::ofstream projectFile;

                fs::path projectPath(project_->folderPath_);
                std::cout << projectPath.string() << std::endl;
                projectPath.append(project_->folderName_+ ".project");
                std::cout << projectPath.string() << std::endl;
                if(!fs::exists(projectPath))
                    throw std::runtime_error("project file \"" + projectPath.string() + "\" not exist");
                projectFile.open(projectPath.string());
                projectFile << projectNode << std::endl;
                projectFile.close();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor::loadProject()
{
    char const * filter[1] = {"*.project"};
    char const *projectFile = tinyfd_openFileDialog("Open Project", "",1, filter,"Project file (*.project)",0);
    if (projectFile)
    {
        fs::path fileFolder(projectFile);
        fileFolder.remove_filename();
        YAML::Node project = YAML::LoadFile(projectFile);
        auto sharedProject = std::make_shared<ProjectData>(project.as<ProjectData>());
        sharedProject->folderPath_ = fileFolder.string();
        // register data folder (if exists)
        sharedProject->dataPath_ = fs::path(fileFolder).append("data");
        setProject(sharedProject);
    }
}

void Editor::saveProject()
{
    for (const auto &file : projectDirectory_->getEditedFiles())
    {
        const fs::path& filepath = file.getFilePath();
        const ObjectDataRef &prototype = projectPrototypeProvider_.getPrototype(file);

        YAML::Node prototypeNode;
        prototypeNode = *prototype;
        std::ofstream prototypeFile;
        prototypeFile.open(filepath.string());
        prototypeFile << prototypeNode << std::endl;
        prototypeFile.close();
    }

    project_->dirty_ = false;
    projectDirectory_->markAllSaved();
    updateWindowTitle();
}

void Editor::setProject(const std::shared_ptr<ProjectData> &project)
{
    project_ = project;
    objectSelected_.reset();

    if(project_->currentScenePath_.empty())
    {
        sceneData_.reset(new SceneData());
        sceneData_->name_ = "New Scene";
    }
    else
    {
        loadScene(project_->currentScenePath_);
    }

    projectPrototypeProvider_.clearCache();
    projectDirectory_.reset(new ProjectDirectory(project_));

    updateWindowTitle();
}

void Editor::loadScene(const std::string &sceneFilePath)
{
    YAML::Node sceneNode = YAML::LoadFile(sceneFilePath);
    SceneData scene = sceneNode.as<SceneData>();
    scene.filePath_ = sceneFilePath;

    //TODO: move all related to "clean" into a method
    objectSelected_.reset();
    projectPrototypeProvider_.clearCache();
    project_->dirty_ = false;
    project_->currentScenePath_ = sceneFilePath;

    sceneData_.reset(new SceneData(scene));

    updateWindowTitle();
}

void Editor::updateWindowTitle()
{
    std::string dirtyMark;
    if(project_->dirty_)
        dirtyMark = "*";
    SDL_SetWindowTitle(window_,("Game Engine Editor - " + project_->folderName_ + " - " + sceneData_->name_ + dirtyMark).c_str());
}

void Editor::deleteFile(const boost::filesystem::path &filePath)
{
    fs::path absolutePath = fs::absolute(filePath, project_->dataPath_);
    if(fs::remove(absolutePath))
    {
        projectDirectory_->removeFile(absolutePath);
        if(objectSelected_->data == projectPrototypeProvider_.deletePrototype(absolutePath.string()))
            objectSelected_.reset();
    }
}

bool Editor::renderFileSelector(const std::string &label, const DataFile &file, DataFile *result)
{
    bool selected = false;
    const std::string &currentFile = file.getFilePath().string();
    if (ImGui::BeginCombo(label.c_str(), currentFile.c_str()))
    {
        std::vector<DataFile> files = projectDirectory_->getFiles(file.getType());
        for(auto item : files)
        {
            const std::string &itemFilepath = item.getFilePath().string();
            bool is_selected = (currentFile == itemFilepath);
            if (ImGui::Selectable(itemFilepath.c_str(), is_selected))
            {
                *result = item;
                selected = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return selected;
}
