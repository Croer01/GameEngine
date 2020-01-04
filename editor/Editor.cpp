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

    auto callback = std::bind(&Editor::setProject, this, std::placeholders::_1);
    createProjectEditor_= std::make_shared<CreateProjectEditor>(callback);

//    generateMockData();
}

void Editor::generateMockData()
{
    sceneData_.reset(new SceneData());

    for(int i = 0; i < 5; i++)
    {
        ObjectData object;
        std::ostringstream stringStream;
        stringStream << "Object" << i;
        object.name_ = stringStream.str();

        for(int j = 0; j < i; j++)
        {
            ObjectData children;
            std::ostringstream stringStream2;
            stringStream2 << "Child" << j;
            children.name_ = stringStream2.str();

            auto component = std::make_shared<ComponentData>();
            component->name_ = "component 1";

            auto propertyData = std::make_shared<PropertyFloatData>("property float 1");
            component->properties_.push_back(propertyData);
            children.components_.push_back(component);
            object.children_.push_back(std::make_shared<ObjectData>(children));
        }

        sceneData_->objects_.push_back(std::make_shared<ObjectData>(object));
    }
}

void Editor::render()
{
    renderMainMenu();
    if(createProjectEditor_->isOpen())
        createProjectEditor_->Render();

    if(project_)
    {
        renderSceneInspector();
        renderPrototypeList();
        renderObjectInspector();
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

void Editor::renderSceneObjectNode(const ObjectDataRef &object, const std::string &id)
{
    const std::string label = object->name_ + "##ObjectNode" + id;

    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(object->children_.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    if(objectSelected_ == object)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    bool isOpened = ImGui::TreeNodeEx(label.c_str(), nodeFlags);
    if(ImGui::IsItemClicked())
        objectSelected_ = object;

    if (isOpened)
    {
        for (auto i = 0; i < object->children_.size(); i++)
        {
            renderSceneObjectNode(object->children_[i], id + "_" + std::to_string(i));
        }
        ImGui::TreePop();
        if((nodeFlags & ImGuiTreeNodeFlags_Leaf) != ImGuiTreeNodeFlags_Leaf)
            ImGui::Separator();
    }
}

void Editor::renderPrototypeList()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 40;
    size.y /= 2.f;
    ImGui::SetNextWindowPos(ImVec2(0,size.y));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Prototypes",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::Separator();
    if(ImGui::Button("Create Prototype"))
    {
        // TODO: implement this
    }

    ImGuiTreeNodeFlags PrototypesNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    for (const auto &filepath : project_->prototypeFilepaths_)
    {
        const boost::filesystem::path &relativeFilePath = fs::relative(fs::path(filepath), project_->dataPath_);
        ImGui::TreeNodeEx(relativeFilePath.string().c_str(), PrototypesNodeFlags);
        if(ImGui::IsItemClicked())
            objectSelected_ = projectPrototypeProvider_.getPrototype(filepath.string());
    }

    ImGui::End();
}

void Editor::renderSceneInspector()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    ImGui::SetNextWindowPos(ImVec2(0,20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Scene Inspector",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(ImGui::InputText("Name", &sceneData_->name_))
    {
        updateWindowTitle();
    }
    ImGui::Separator();
    if(ImGui::Button("New Object"))
    {
        ObjectDataRef newObject = std::make_shared<ObjectData>();
        std::stringstream ss;
        ss << "Object" << sceneData_->objects_.size();
        newObject->name_ = ss.str();
        sceneData_->objects_.push_back(newObject);
    }

    for (auto i = 0; i < sceneData_->objects_.size(); i++)
    {
        renderSceneObjectNode(sceneData_->objects_[i], std::to_string(i));
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

    std::string objectLabel = objectSelected_? objectSelected_->name_ : "Empty";

    ImGui::Begin((objectLabel + " - Properties Inspector###PropertyInspector").c_str(), 0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(objectSelected_)
    {
        ImGui::InputText("Name",&objectSelected_->name_);
        if(ImGui::Button("New Child"))
        {
            ObjectDataRef newChild = std::make_shared<ObjectData>();
            std::stringstream ss;
            ss << "Child" << objectSelected_->children_.size();
            newChild->name_ = ss.str();
            objectSelected_->children_.push_back(newChild);
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
            assert(objectSelected_);
            ComponentDataRef newComponent = std::make_shared<ComponentData>();
            newComponent->name_ = item_current;
            newComponent->properties_ = gameComponentsProvider_.getPropertiesMetadata(item_current);

            objectSelected_->components_.push_back(newComponent);
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat2("position", objectSelected_->position_.xy.data(),.1f);
            ImGui::DragFloat2("size", objectSelected_->scale_.xy.data(), .1f, 1.f, std::numeric_limits<float>::max());
            ImGui::DragFloat2("rotation", objectSelected_->rotation_.xy.data(),.1f);
        }

        for (const auto &component : objectSelected_->components_)
        {
            ImGui::PushID(objectSelected_->name_.c_str());
            renderComponent(component);
            ImGui::PopID();
        }
    }
    ImGui::End();
}

void Editor::renderComponent(const ComponentDataRef &component)
{
    if (ImGui::CollapsingHeader(component->name_.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushID(component->name_.c_str());
        for (const auto &property : component->properties_)
        {
            ImGui::PushID(property->name_.c_str());
            switch (property->type_)
            {
                case PropertyDataType::INT:
                {
                    auto propertyInt = std::dynamic_pointer_cast<PropertyIntData>(property);
                    ImGui::DragInt(property->name_.c_str(), &propertyInt->value_, .1f);
                }
                    break;
                case PropertyDataType::FLOAT:
                {
                    auto propertyFloat = std::dynamic_pointer_cast<PropertyFloatData>(property);
                    ImGui::DragFloat(property->name_.c_str(), &propertyFloat->value_, .1f);
                }
                    break;
                case PropertyDataType::STRING:
                {
                    auto propertyString = std::dynamic_pointer_cast<PropertyStringData>(property);
                    ImGui::InputText(propertyString->name_.c_str(), &propertyString->value_);
                }
                    break;
                case PropertyDataType::BOOL:
                {
                    auto propertyBool = std::dynamic_pointer_cast<PropertyBoolData>(property);
                    ImGui::Checkbox(property->name_.c_str(), &propertyBool->value_);
                }
                    break;
                case PropertyDataType::VEC2D:
                {
                    auto propertyVec2D = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    ImGui::DragFloat2(property->name_.c_str(), propertyVec2D->value_.xy.data());
                }
                    break;
                case PropertyDataType::COLOR:
                {
                    auto propertyColor = std::dynamic_pointer_cast<PropertyColorData>(property);
                    ImGui::ColorEdit3(property->name_.c_str(), propertyColor->value_.rgb.data());
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
                    }

                    auto values = propertyStringArray->value_;
                    for (int i = 0; i < values.size(); ++i)
                    {
                        ImGui::PushID(i);
                        ImGui::InputText("", &values[i]);
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
                    }

                    auto &values = propertyVec2DArray->value_;
                    for (int i = 0; i < values.size(); ++i)
                    {
                        ImGui::PushID(i);
                        Vector2DData &value = values[i];
                        ImGui::DragFloat2("", value.xy.data());
                        ImGui::PopID();
                    }
                }
                    break;
            }
            ImGui::PopID();
        }
        ImGui::PopID();
    }
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
                    YAML::Node scene = YAML::LoadFile(sceneFile);
                    const SceneData &data = scene.as<SceneData>();
                    sceneData_.reset(new SceneData(data));
                    sceneData_->filePath_ = sceneFile;
                    objectSelected_.reset();
                    updateWindowTitle();
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
        SceneData sceneLoaded = loadScene(project_->currentScenePath_);
        sceneData_.reset(new SceneData(sceneLoaded));
    }

    // List the data files
    // TODO: observe directory to update list if some file is created/deleted
    projectPrototypeProvider_.clearCache();
    if(fs::exists(project_->dataPath_) && fs::is_directory(project_->dataPath_))
    {
        fs::directory_iterator end;
        for (fs::directory_iterator itr(project_->dataPath_); itr != end; ++itr)
        {
            //TODO: recursive directories?
            if (is_regular_file(itr->path()) && itr->path().extension() == ".prototype")
            {
                project_->prototypeFilepaths_.push_back(itr->path());
            }
        }
    }

    updateWindowTitle();
}

SceneData Editor::loadScene(const std::string &sceneFilePath)
{
    YAML::Node sceneNode = YAML::LoadFile(sceneFilePath);
    SceneData scene = sceneNode.as<SceneData>();
    return std::move(scene);
}

void Editor::updateWindowTitle()
{
    SDL_SetWindowTitle(window_,("Game Engine Editor - " + project_->folderName_ + " - " + sceneData_->name_).c_str());
}