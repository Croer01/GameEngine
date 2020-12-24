//
// Created by adria on 11/10/2019.
//

#include <imgui.h>
#include <chrono>
#include "imgui_backend/imgui_stdlib.h"
#include <memory>
#include <sstream>
#include "Editor.hpp"
#include "../../componentsRegistered.hpp"
#include "GameEditor.hpp"
#include <functional>
#include <boost/filesystem/path.hpp>
#include <tinyfiledialogs.h>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <game-engine/geEnvironment.hpp>

namespace fs = boost::filesystem;
using namespace std::chrono_literals;


Editor::Editor(SDL_Window *window, SDL_GLContext glContext) :
    window_(window),
    glContext_(glContext)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_FrameBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.09f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.00f, 0.92f, 1.00f, 1.00f);




    ImGui::GetStyle().WindowRounding = 0.f;
    ImGui::GetStyle().FrameRounding = 3.f;

    createProjectEditor_= std::make_shared<CreateProjectEditor>([=] (const ProjectDataRef &projectData) { setProject(projectData); });
    createDataFileDialog_ = std::make_shared<CreateDataFileDialog>([=] (const boost::filesystem::path &prototypePath) {
        if(fs::extension(prototypePath) == ".prototype")
            createPrototype(prototypePath);
        else if(fs::extension(prototypePath) == ".scene")
            createScene(prototypePath);
    });
    deleteFileDialog_ = std::make_shared<DeleteFileDialog>([=] (const boost::filesystem::path &filePath){ deleteFile(filePath); });
    saveAllDialog_ = std::make_shared<SaveAllDialog>([=](const fs::path &result){
        saveProject();
        // the result is the scene file we want to load. This is used when choose a scene but there are changes to save
        if(!result.empty())
            loadScene(result.string());
        else
            reloadGameStaticScene();
    });
    errorDialog_ = std::make_shared<ErrorDialog>();

    gameGlContext_ = SDL_GL_CreateContext(window_);
    // restore the gl context after create the gl context to use in game thread
    SDL_GL_MakeCurrent(window_, glContext_);
}

void Editor::render()
{
    renderMainMenu();

    createProjectEditor_->Render();
    createDataFileDialog_->Render();
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
        renderDataDirectoryExplorer();
        renderPrototypeInspector();
        renderCentralRegion();

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

bool Editor::renderSceneObjectNode(const PrototypeReferenceRef &object, const std::string &id, bool &deleteObject)
{
    bool edited = false;

    ImGui::PushID(("ObjectNode" + id).c_str());
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(sceneData_->getSelectedObject() == object)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool showTreeContent = ImGui::TreeNodeEx(id.c_str(), flags, "%s", object->name_.c_str());
    if(ImGui::IsItemClicked())
    {
        sceneData_->selectObject(object);
    }
    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 3);

    if(ImGui::Button("X"))
    {
        deleteObject = true;
        edited = true;
    }

    if (showTreeContent)
    {
        ImGui::InputText("Name",&object->name_);
        if(ImGui::IsItemEdited())
            edited = true;

        ImGui::InputText("prototype",&object->prototype_,ImGuiInputTextFlags_ReadOnly);
        if(ImGui::IsItemEdited())
            edited = true;

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            Vector2DData position = object->getPosition();
            if(ImGui::DragFloat2("position", position.xy.data(), .1f))
            {
                object->setPosition(position);
                edited = true;
            }
            Vector2DData scale = object->getScale();
            if(ImGui::DragFloat2("size", scale.xy.data(), .1f, 1.f, std::numeric_limits<float>::max()))
            {
                object->setScale(scale);
                edited = true;
            }
            float rotation = object->getRotation();
            if(ImGui::DragFloat("rotation", &rotation,.1f))
            {
                object->setRotation(rotation);
                edited = true;
            }
        }

        ImGui::TreePop();
        ImGui::Separator();
    }
    ImGui::PopID();

    return edited;
}

void Editor::renderPrototypeListInternal(const DataDirectoryRef &dir)
{
    ImGui::PushID("files");
    for(const auto &file : dir->getFiles())
    {
        ImGui::PushID(file->getFilePath().c_str());
        ImGuiTreeNodeFlags PrototypesNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
        ImGui::TreeNodeEx(file->getFilePath().filename().string().c_str(), PrototypesNodeFlags);
        if(ImGui::IsItemClicked())
        {
            DataFile &dataFile = *file.get();
            switch (file->getType())
            {
                case DataFileType::Prototype:
                    try {
                        auto object = new TargetObject();
                        object->data = projectFileDataProvider_.getObjectData(dataFile);
                        object->sourceFile = dataFile;

                        objectSelected_.reset(object);
                    }
                    catch (const std::exception &e)
                    {
                        errorDialog_->open(e.what());
                    }
                    break;
                case DataFileType::Scene:
                    try {
                        if(projectDirectory_->hasEditedFiles())
                        {
                            saveAllDialog_->setFilesToSave(projectDirectory_->getEditedFiles());
                            saveAllDialog_->open(file->getFilePath());
                        }
                        else
                        {
                            loadScene(file->getFilePath().string());
                        }
                    }
                    catch (const std::exception &e)
                    {
                        errorDialog_->open(e.what());
                    }
                    break;
            }
        }
        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 3);
        if(ImGui::Button("X"))
        {
            const boost::filesystem::path &relativeFilePath = file->getFilePath();
            deleteFileDialog_->open(relativeFilePath);
        }
        ImGui::PopID();
    }
    ImGui::PopID();

    ImGui::PushID("dirs");
    for(const auto &subdir : dir->getFolders())
    {
        ImGui::PushID(subdir->name().c_str());
        bool opened = ImGui::TreeNode(subdir->name().c_str());

        float labelWidth = ImGui::CalcTextSize("Create...").x;
        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - labelWidth);
        if(ImGui::Button("Create..."))
        {
            createDataFileDialog_->open(subdir->getDirectoryPath(), "NewPrototype");
        }

        if(opened)
        {
            renderPrototypeListInternal(subdir);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::PopID();
}

void Editor::renderDataDirectoryExplorer()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    size.y /= 2.f;
    ImGui::SetNextWindowPos(ImVec2(0,size.y + 20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Data Explorer",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGuiTreeNodeFlags PrototypesNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    renderPrototypeListInternal(projectDirectory_->getTree());

    ImGui::End();
}

void Editor::createPrototype(const boost::filesystem::path &prototypeFilepath)
{
    fs::path prototypePath(this->project_->dataPath_);

    prototypePath /= prototypeFilepath;

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

void Editor::createScene(const boost::filesystem::path &sceneFilepath)
{
    fs::path scenePath(this->project_->dataPath_);

    scenePath /= sceneFilepath;

    SceneData newScene;
    newScene.name_ = scenePath.stem().string();

    YAML::Node sceneNode;
    sceneNode = newScene;
    std::ofstream sceneFile;
    sceneFile.open(scenePath.string());
    sceneFile << sceneNode << std::endl;
    sceneFile.close();
    projectDirectory_->addFile(scenePath);

    // Load created scene
    loadScene(scenePath.string());
}

void Editor::renderSceneInspector()
{
    bool dirty = false;

    ImVec2 size = ImGui::GetIO().DisplaySize;
    size.x *= 0.25f;
    size.y -= 20;
    size.y /= 2.f;
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Scene Inspector",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if(ImGui::InputText("Name", &sceneData_->name_))
    {
        dirty = true;
        updateWindowTitle();
    }
    ImGui::Separator();
    static DataFile currentFile = projectDirectory_->getFiles(DataFileType::Prototype)[0];
    if (ImGui::BeginCombo("Prototype",
                          currentFile.getFilePath().string().c_str())) // The second parameter is the label previewed before opening the combo.
    {
        std::vector<DataFile> files = projectDirectory_->getFiles(DataFileType::Prototype);
        for(auto item : files)
        {
            bool is_selected = (currentFile == item);
            if (ImGui::Selectable(item.getFilePath().string().c_str(), is_selected))
                currentFile = item;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if(ImGui::Button("New Object"))
    {
        PrototypeReferenceRef newObject = std::make_shared<PrototypeReference>();
        std::stringstream ss;
        ss << "Object" << sceneData_->objectsSize();
        newObject->name_ = ss.str();
        const ObjectDataRef &ptr = projectFileDataProvider_.getObjectData(currentFile);
        newObject->prototype_ = ptr->name_;
        sceneData_->addObject(newObject);
        dirty = true;
    }

    ImGui::Separator();

    auto objectToDeleteId = -1;
    for (auto i = 0; i < sceneData_->objectsSize(); i++)
    {
        bool deleteObject = false;
        if(renderSceneObjectNode(sceneData_->getObject(i), std::to_string(i), deleteObject))
        {
            dirty = true;
        }

        if(deleteObject)
            objectToDeleteId = i;
    }

    if(objectToDeleteId != -1)
    {
        sceneData_->deleteObject(objectToDeleteId);
    }

    ImGui::End();

    if(dirty)
    {
        project_->dirty_ = true;
        projectDirectory_->markEdited(DataFile(sceneData_->filePath_));
    }
}

void Editor::renderPrototypeInspector()
{
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
        bool edited = renderObjectNode(objectSelected_->data);
        if(edited) {
            project_->dirty_ = true;
            projectDirectory_->markEdited(objectSelected_->sourceFile);
        }
    }
    ImGui::End();
}


bool Editor::renderObjectNode(const ObjectDataRef &object)
{
    bool edited = false;

    ImGui::InputText("Name", &object->name_);
    if (ImGui::IsItemEdited())
        edited = true;

    ImGui::PushID(object->name_.c_str());

    if (ImGui::Button("New Child"))
    {
        ObjectDataRef newChild = std::make_shared<ObjectData>();
        std::stringstream ss;
        ss << "Child" << object->children_.size();
        newChild->name_ = ss.str();
        object->children_.push_back(newChild);
        edited = true;
    }

    ImGui::Separator();
    static std::string item_current = gameComponentsProvider_.getRegisteredPropertiesIds()[0];
    if (ImGui::BeginCombo("Component",
                          item_current.c_str())) // The second parameter is the label previewed before opening the combo.
    {
        auto components = gameComponentsProvider_.getRegisteredPropertiesIds();
        for (std::string &component : components)
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

    if (ImGui::Button("Add Component..."))
    {
        assert(object);
        ComponentDataRef newComponent = std::make_shared<ComponentData>();
        newComponent->name_ = item_current;
        newComponent->properties_ = gameComponentsProvider_.getPropertiesMetadata(item_current);

        object->components_.push_back(newComponent);
        edited = true;
    }

    ImGui::Separator();
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat2("position", object->position_.xy.data(), .1f))
            edited = true;
        if (ImGui::DragFloat2("size", object->scale_.xy.data(), .1f, 1.f, std::numeric_limits<float>::max()))
            edited = true;
        if (ImGui::DragFloat("rotation", &object->rotation_, .1f))
            edited = true;
    }

    int i = 0;
    int removeComponent = -1;
    int moveComponent = 0;
    for (const auto &component : object->components_)
    {
        ImGui::PushID(i);
        if (renderComponent(component, i, object->components_.size(), &moveComponent, &removeComponent))
            edited = true;
        ImGui::PopID();
        i++;
    }
    ImGui::PopID();

    // This code doesn't manage the case that multiple components are removed or moved at the same time.
    // Simply because the GUI doesn't allow to do that and simplify the code.

    // move component
    // Negative means move up the element, positive means move down. The value is the position, not the array index
    // eg: if moveComponent is -3, the array[2] element will move up
    if(moveComponent > 0)
    {
        int index = moveComponent - 1;
        std::iter_swap(object->components_.begin() + index, object->components_.begin() + (index + 1));
    }
    else if(moveComponent < 0)
    {
        int index = std::abs(moveComponent) - 1;
        std::iter_swap(object->components_.begin() + index, object->components_.begin() + (index - 1));
    }

    // remove components safety
    if (removeComponent != -1)
    {
        object->components_.erase(object->components_.begin() + removeComponent);
        edited = true;
    }

    // render children

    for (const auto &child : object->children_)
    {
        ImGui::PushID(child.get());
        if (ImGui::TreeNodeEx(child.get(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth, "%s", child->name_.c_str()))
        {
            bool childEdited = renderObjectNode(child);
            edited = edited || childEdited;
            ImGui::TreePop();
            ImGui::Separator();
        }
        ImGui::PopID();
    }
    return edited;
}

void Editor::renderCentralRegion()
{
    ImVec2 size = ImGui::GetIO().DisplaySize;
    float posX = size.x *0.25f;
    size.x *= 0.5f;
    size.y -= 20;
    ImGui::SetNextWindowPos(ImVec2(posX, 20));
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Central",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (sceneData_ && ImGui::BeginTabItem("Scene viewer"))
        {
            renderSceneViewer();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Physics"))
        {
            renderPhysicsInspector();
            ImGui::EndTabItem();
        }

        if (objectSelected_ && ImGui::BeginTabItem("GUI editor"))
        {
            renderGuiInspector();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Editor::renderGuiInspector()
{
    if(!objectSelected_)
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 &windowSize = ImGui::GetContentRegionAvail();
    ImColor foreground;
    ImColor background;
    std::string text;
    float fontSize = 0;

    for (const auto &component : objectSelected_->data->components_)
    {
        foreground = ImColor(0, 0, 0);
        background = ImColor(48, 218, 48);
        text.clear();
        fontSize = ImGui::GetFontSize();

        if(component->name_.find("UI") != -1)
        {
            Vector2DData screenPos;
            Vector2DData screenSize;

            for (const auto &property : component->properties_)
            {
                if(property->name_ == "screenPos")
                {
                    auto propertyVec2D = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    screenPos = propertyVec2D->value_;
                }
                else if(property->name_ == "screenSize")
                {
                    auto propertyVec2D = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    screenSize = propertyVec2D->value_;
                }
                else if (property->name_ == "background")
                {
                    auto propertyColor = std::dynamic_pointer_cast<PropertyColorData>(property);
                    std::array<float, 3> &colorComponents = propertyColor->value_.rgb;
                    background = ImColor(colorComponents[0], colorComponents[1], colorComponents[2]);
                }
                else if (property->name_ == "foreground")
                {
                    auto propertyColor = std::dynamic_pointer_cast<PropertyColorData>(property);
                    std::array<float, 3> &colorComponents = propertyColor->value_.rgb;
                    foreground = ImColor(colorComponents[0], colorComponents[1], colorComponents[2]);
                }
                else if (property->name_ == "text")
                {
                    auto propertyString = std::dynamic_pointer_cast<PropertyStringData>(property);
                    text = propertyString->value_;
                }
                else if (property->name_ == "fontSize")
                {
                    auto propertyInt = std::dynamic_pointer_cast<PropertyIntData>(property);
                    if(propertyInt->value_ != 0)
                        fontSize = propertyInt->value_;
                }
            }


            // Cursor means the position which ImGui will draw the next element
            ImVec2 controlPos = ImGui::GetCursorScreenPos();
            controlPos.x += screenPos.xy[0] * windowSize.x;
            controlPos.y += screenPos.xy[1] * windowSize.y;
            ImVec2 controlSize = controlPos;
            controlSize.x += (screenSize.xy[0] * windowSize.x);
            controlSize.y += (screenSize.xy[1] * windowSize.y);


            if(component->name_.find("Button") != -1 || component->name_.find("Panel") != -1)
                draw_list->AddRectFilled(controlPos, controlSize, background);
            else
                draw_list->AddRect(controlPos, controlSize, background);

            if(!text.empty())
                draw_list->AddText(ImGui::GetFont(), fontSize, controlPos, foreground, text.c_str());
        }
    }
}

void Editor::renderPhysicsInspector()
{
    if(!project_)
        return;

    ImGui::PushID("Physics");
    std::vector<PhysicsCategory> &categories = project_->physicsCategories_;

    // -- BEGIN ADD CATEGORY --
    static std::string newCategoryName;
    ImGui::InputText("Category", &newCategoryName);
    ImGui::SameLine();
    if(ImGui::Button("Add"))
    {
        PhysicsCategory physicsCategory;
        physicsCategory.name_ = newCategoryName;
        // TODO: avoid add duplicates
        categories.push_back(physicsCategory);
        project_->dirty_ = true;
    }
    ImGui::NewLine();
    // -- END ADD CATEGORY --

    std::string categoryToDelete;

    bool window_visible = ImGui::BeginChild(ImGui::GetID("Physics"),ImVec2(0,0), false, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
    if(window_visible)
    {
        // Calculate the width of the first column
        float firstColumnWidth = 0.f;
        float firstRowHeight = 0.f;
        float otherColumnWidth = ImGui::GetFrameHeight(); // the size of a checkbox (get from imgui source code)
        for(const auto &category : categories)
        {
            const ImVec2 &textSize = ImGui::CalcTextSize(category.name_.c_str());
            if(firstColumnWidth < textSize.x)
                firstColumnWidth = textSize.x;
        }
        // at this point the first row will have the same height as the width of the first column
        firstRowHeight = ImGui::GetCursorPosY() + firstColumnWidth;

        // Add the space between column + label + button
        const float buttonWidth = 16.f;
        // TODO: Maybe add one extra pixel to draw a border line between columns?
        firstColumnWidth += ImGui::GetStyle().FramePadding.x * 4.f + buttonWidth;
        otherColumnWidth += ImGui::GetStyle().FramePadding.x * 2.f;
        ImGui::SameLine(firstColumnWidth);
        {
            const ImVec2 &textSize = ImGui::CalcTextSize("all");
            ImGui::SetCursorPosY(firstRowHeight - textSize.x);
            ImGui::VerticalText("all");
        }

        float currentLabelPos = firstColumnWidth + otherColumnWidth;
        for(const auto &category : categories)
        {
            const ImVec2 &textSize = ImGui::CalcTextSize(category.name_.c_str());
            ImGui::SameLine(currentLabelPos);
            ImGui::SetCursorPosY(firstRowHeight - textSize.x);
            ImGui::VerticalText(category.name_.c_str());
            currentLabelPos += otherColumnWidth;
        }
        // Reset the Y Cursor to draw correctly the new line
        ImGui::SetCursorPosY(firstRowHeight);
        ImGui::NewLine();

        for(auto &category : categories)
        {
            ImGui::PushID(category.name_.c_str());

            // Draw label and delete button
            ImGui::Text("%s", category.name_.c_str());
            ImGui::SameLine(firstColumnWidth - buttonWidth - ImGui::GetStyle().FramePadding.x);
            if(ImGui::Button("x"))
            {
                categoryToDelete = category.name_;
            }

            ImGui::SameLine();

            // -- BEGIN CATEGORY OPTIONS COLUMNS --
            // "all" option column
            bool allChecked = category.mask_.empty(); //empty mask means check collisions with all
            if(ImGui::Checkbox("", &allChecked))
            {
                if(allChecked)
                {
                    category.mask_.clear();

                    for(auto &other : categories)
                    {
                        auto itOther = std::remove_if(other.mask_.begin(), other.mask_.end(),
                                                      [&](const auto &c)
                                                      {
                                                          return c == category.name_;
                                                      });

                        if (itOther != other.mask_.end())
                        {
                            other.mask_.erase(itOther, other.mask_.end());
                        }
                    }
                    project_->dirty_ = true;
                }
            }
            ImGui::SameLine();

            // category columns
            for(auto &other : categories)
            {
                ImGui::PushID(other.name_.c_str());

                if(category.name_ != other.name_)
                {
                    auto it = std::find(category.mask_.begin(), category.mask_.end(), other.name_);
                    bool checked = it != category.mask_.end();

                    if (ImGui::Checkbox("", &checked))
                    {
                        if (checked)
                        {
                            category.mask_.push_back(other.name_);
                            other.mask_.push_back(category.name_);
                        }
                        else
                        {
                            category.mask_.erase(it);
                            auto itOther = std::find(other.mask_.begin(), other.mask_.end(), category.name_);
                            assert(itOther != other.mask_.end());
                            other.mask_.erase(itOther);
                        }
                        project_->dirty_ = true;
                    }

                    ImGui::SameLine();
                }
                else
                {
                    ImGui::SameLine(ImGui::GetCursorPosX() + otherColumnWidth);
                }
                ImGui::PopID();
            }
            // -- END CATEGORY OPTIONS COLUMNS --

            // This dummy is to suppress the last SameLine
            ImGui::Dummy(ImVec2());
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    // -- BEGIN DELETE CATEGORY LOGIC --
    if(!categoryToDelete.empty())
    {
        auto it = std::remove_if(categories.begin(), categories.end(),
                                 [&](const auto &c)
                                 {
                                     return c.name_ == categoryToDelete;
                                 });
        assert(it != categories.end());
        categories.erase(it, categories.end());

        for(auto &category : categories)
        {
            auto itOther = std::remove_if( category.mask_.begin(), category.mask_.end(),
                                      [&](const auto &c)
                                      {
                                          return c == categoryToDelete;
                                      });

            if(itOther != category.mask_.end()){
                category.mask_.erase(itOther, category.mask_.end());
            }
        }

        project_->dirty_ = true;
    }
    // -- END DELETE CATEGORY LOGIC --

    // reset columns
    ImGui::Columns();
    ImGui::PopID();
}

bool Editor::renderComponent(const ComponentDataRef &component, int index, int length, int *moveComponent, int *removeComponent)
{
    bool edited = false;
    ImGui::PushID(component->name_.c_str());
    bool opened = ImGui::CollapsingHeader(component->name_.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap);
    float reserveSpaceForButtons = 0.f;
    if(index != 0)
        reserveSpaceForButtons += 23.f;
    if(index != length -1)
        reserveSpaceForButtons += 23.f;
    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 3 - reserveSpaceForButtons);
    if(index != 0)
    {
        if (ImGui::Button("^"))
        {
            (*moveComponent) = (index + 1) * -1;
            edited = true;
        }
        ImGui::SameLine();
    }
    if(index != length -1)
    {
        if (ImGui::Button("v"))
        {
            (*moveComponent) = index + 1;
            edited = true;
        }
        ImGui::SameLine();
    }
    if(ImGui::Button("x"))
    {
        (*removeComponent) = index;
        edited = true;
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
                    // Always include the first line
                    float numLines = 1 + std::count(propertyString->value_.begin(), propertyString->value_.end(), '\n');
                    // Arbitrary limit to avoid text block grows indefinitely
                    if(numLines > 10)
                        numLines = 10;
                    ImGui::InputTextMultiline(propertyString->name_.c_str(), &propertyString->value_, ImVec2(0, ImGui::GetFrameHeight() + ImGui::GetTextLineHeight() * numLines));
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

                    if(property->name_ == "screenPos" || property->name_ == "screenSize")
                        ImGui::DragFloat2(property->name_.c_str(), propertyVec2D->value_.xy.data(),0.001f, .0f, 1.f);
                    else
                        ImGui::DragFloat2(property->name_.c_str(), propertyVec2D->value_.xy.data());

                    if(ImGui::IsItemEdited())
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
                    if(renderFileSelector(propertyFilePath->name_.c_str(),
                            propertyFilePath->value_.getFilePath().string(),
                            propertyFilePath->fileTypeSupported_,
                            &fileSelected))
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
                reloadGameStaticScene();
            }
            ImGui::Separator();

            std::string sceneName = "Scene - ";
            if(sceneData_)
                sceneName += sceneData_->name_;
            else
                sceneName += "[Empty]";

            ImGui::MenuItem(sceneName.c_str(), NULL, false, false);
            // TODO: review this menu entry. Do I need this?
            // if (ImGui::MenuItem("Create...", NULL, false, (bool)sceneData_))
            // {
            //     const std::string &defaultValue =
            //         "Scene" + std::to_string(projectDirectory_->getFiles().size());
            //     createSceneDialog_->open(defaultValue);
            // }

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

                // The scene has saved from memory copy, so that, we load the sceneData again from file to link with the project data management system
                if(saveAs)
                {
                    projectDirectory_->addFile(sceneData_->filePath_);
                    sceneData_ = projectFileDataProvider_.getSceneData(DataFile(sceneData_->filePath_));
                }

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

        static bool openDemoWindow = false;
        if(ImGui::MenuItem("Imgui Demo"))
        {
            openDemoWindow = true;
        }

        if(openDemoWindow)
            ImGui::ShowDemoWindow(&openDemoWindow);

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

        fs::path physicsPath = fs::path(fileFolder).append("conf").append("physics.yaml");
        if(fs::exists(physicsPath))
        {
            YAML::Node physicsNode = YAML::LoadFile(physicsPath.string());
            std::vector<PhysicsCategory> categories;

            if (physicsNode["categories"])
            {
                YAML::Node categoriesNode = physicsNode["categories"];

                categories.reserve(categoriesNode.size());
                for (int i = 0; i < categoriesNode.size(); ++i)
                {
                    PhysicsCategory tmpCategory;
                    tmpCategory.name_ = categoriesNode[i].as<std::string>();

                    if (physicsNode["masks"])
                    {
                        YAML::Node masksNode = physicsNode["masks"][tmpCategory.name_];
                        if (masksNode.IsSequence())
                        {
                            for (int j = 0; j < masksNode.size(); ++j)
                            {
                                tmpCategory.mask_.push_back(masksNode[j].as<std::string>());
                            }
                        }
                    }
                    categories.push_back(tmpCategory);
                }
            }
            sharedProject->physicsCategories_.swap(categories);
        }

        setProject(sharedProject);
    }
}

void Editor::saveProject()
{
    // save files
    for (const auto &file : projectDirectory_->getEditedFiles())
    {
        YAML::Node fileDataNode;
        const fs::path& filepath = file.getFilePath();
        if(file.getType() == DataFileType::Prototype)
        {
            const ObjectDataRef &prototype = projectFileDataProvider_.getObjectData(file);
            fileDataNode = *prototype;
        }
        else if(file.getType() == DataFileType::Scene)
        {
            const SceneDataRef &scene = projectFileDataProvider_.getSceneData(file);
            fileDataNode = *scene;
        }
        else
        {
            throw std::runtime_error("Unsupported file. Can't save file " + file.getFilePath().string());
        }
        std::ofstream prototypeFile;
        prototypeFile.open(fs::absolute(filepath, project_->dataPath_).string());
        prototypeFile << fileDataNode << std::endl;
        prototypeFile.close();
    }

    projectDirectory_->markAllSaved();

    // save configuration

    YAML::Node physicConfigNode;
    YAML::Node categoriesNode = physicConfigNode["categories"];
    YAML::Node masksNode = physicConfigNode["masks"];
    for(const auto &config : project_->physicsCategories_)
    {
        if(masksNode[config.name_])
            throw std::runtime_error("Category " + config.name_ + " duplicated, it is already saved.");

        categoriesNode.push_back(config.name_);
        masksNode[config.name_] = YAML::Node(config.mask_);
    }

    {
        std::ofstream physicsConfFile;
        fs::path physicsConfPath = fs::path(project_->folderPath_).append("conf").append("physics.yaml");
        physicsConfFile.open(physicsConfPath.string());
        physicsConfFile << physicConfigNode << std::endl;
        physicsConfFile.close();
    }

    {
        std::ofstream projectFile;

        fs::path projectPath(project_->folderPath_);
        projectPath.append(project_->folderName_+ ".project");
        if(!fs::exists(projectPath))
            throw std::runtime_error("project file \"" + projectPath.string() + "\" not exist");

        project_->currentScenePath_ = fs::absolute(sceneData_->filePath_, project_->dataPath_).string();
        auto projectNode = YAML::Node(*project_);
        projectFile.open(projectPath.string());
        projectFile << projectNode << std::endl;
        projectFile.close();
    }

    auto gameEditor = std::dynamic_pointer_cast<GameEditor>(game_);
    if (gameEditor)
        gameEditor->setDirty(false);

    project_->dirty_ = false;
    updateWindowTitle();
}

void Editor::setProject(const std::shared_ptr<ProjectData> &project)
{
    project_ = project;
    projectFileDataProvider_.setProject(project);

    //TODO: advert about the missed scene?
    fs::path scenePath = fs::path(project_->currentScenePath_);
    if(project_->currentScenePath_.empty() || !fs::exists(scenePath))
    {
        sceneData_ = std::make_shared<SceneData>();
        sceneData_->name_ = "New Scene";
        objectSelected_.reset();
        projectFileDataProvider_.clearCache();
    }
    else
    {
        loadScene(project_->currentScenePath_);
    }

    projectDirectory_ = std::make_unique<ProjectDirectory>(project_);
    createDataFileDialog_->setDataPath(project_->dataPath_);
    updateWindowTitle();
}

void Editor::loadScene(const std::string &sceneFilePath)
{
    //TODO: move all related to "clean" into a method
    objectSelected_.reset();
    projectFileDataProvider_.clearCache();
    project_->dirty_ = false;
    project_->currentScenePath_ = sceneFilePath;

    sceneData_ = projectFileDataProvider_.getSceneData(DataFile(sceneFilePath));

    updateWindowTitle();

    reloadGameStaticScene();
}

void Editor::reloadGameStaticScene()
{
    bool hasContext = static_cast<bool>(renderMutex_);

    if(hasContext)
        releaseCurrentContext();
    if(game_ && game_->isRunning())
    {
        game_->shutdown();

        if(gameThread_.valid())
            gameThread_.wait();
    }

    starGame(false);

    if(hasContext)
        makeCurrentContext();
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
        projectFileDataProvider_.deleteData(absolutePath.string());
        if(objectSelected_ && objectSelected_->sourceFile.getFilePath() == absolutePath)
            objectSelected_.reset();
    }
}

bool Editor::renderFileSelector(const std::string &label, const std::string &currentFile, DataFileType typeSupported, DataFile *result)
{
    bool selected = false;
    if (ImGui::BeginCombo(label.c_str(), currentFile.c_str()))
    {
        std::vector<DataFile> files = projectDirectory_->getFiles(typeSupported);
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

void Editor::setPosToColumnCenter(float width)
{
    // based on https://stackoverflow.com/a/58052701/6952678
    float column = ImGui::GetColumnWidth() / 2.f;
    float scrollX = ImGui::GetScrollX();
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + column - scrollX - spacing - (width/2.f));
}


void Editor::renderSceneViewer()
{
    if(!sceneData_)
        return;

    if(gameThread_.valid() && gameThread_.wait_for(0ms) == std::future_status::ready)
    {
        if(gameThread_.get() != 0)
        {
            throw std::runtime_error("Game closed by an internal error");
        }
    }

    if(auto gameEditor = std::dynamic_pointer_cast<GameEditor>(game_))
    {
        if (ImGui::Button("run game"))
        {
            bool hasContext = static_cast<bool>(renderMutex_);

            if(hasContext)
                releaseCurrentContext();

            game_->shutdown();
            gameThread_.wait();
            starGame(true);

            if(hasContext)
                makeCurrentContext();
        }

        if (gameEditor && gameEditor->isDirty())
        {
            project_->dirty_ = true;
            projectDirectory_->markEdited(DataFile(sceneData_->filePath_));
        }
    }
    else if(game_ && game_->isRunning())
    {
        if (ImGui::Button("stop game"))
        {
            bool hasContext = static_cast<bool>(renderMutex_);

            if(hasContext)
                releaseCurrentContext();

            game_->shutdown();
            gameThread_.wait();
            starGame(false);

            if(hasContext)
                makeCurrentContext();
        }
    }

    if(game_)
    {
        GameEngine::geScreen *screen = game_->screen();
        const int w = screen->windowWidth();
        const int h = screen->windowHeight();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Ask ImGui to draw it as an image:
        // Under OpenGL the ImGUI image type is GLuint
        // So make sure to use "(void *)tex" but not "&tex"
        ImGui::GetWindowDrawList()->AddImage(
            (void *)game_->getRenderer(),
            pos,
            ImVec2(pos.x + w, pos.y + h),
            ImVec2(-1, 1),
            ImVec2(0, 0));

        screen->setWindowRelativePosition(pos.x, pos.y);
    }
}

void Editor::starGame(bool update)
{
    gameThread_ = std::async(std::launch::async, [&, update]()
    {
        int exitStatus = 0;
        try
        {
            if (sceneData_->filePath_.empty())
            {
                errorDialog_->open("There is no scene loaded or the current scene is not saved");
                return exitStatus;
            }

            GameEngine::geEnvironmentRef env = GameEngine::geEnvironment::createInstance();
            env->setMakeCurrentContextCallback([=](){
                SDL_GL_MakeCurrent(window_, gameGlContext_);
            });
            env->configurationPath(project_->folderPath_ + "/conf");

            RegisterComponents(env);
            env->addResourcesFromPath(project_->dataPath_.string());

            env->firstScene(sceneData_->name_);

            if(update)
                game_ = GameEngine::Game::createInstance(env);
            else
            {
                auto envInternal = std::dynamic_pointer_cast<GameEngine::Internal::Environment>(env);
                const std::shared_ptr<GameEditor> &gameEditor = std::make_shared<GameEditor>(envInternal);
                game_ = gameEditor;
                gameEditor->linkSceneFromEditor(sceneData_);
            }

            game_->init();
            while(game_->isRunning())
            {
                game_->update();
                game_->render();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            exitStatus = 1;
        }

        game_.reset();
        return exitStatus;
    });
}

void Editor::releaseCurrentContext()
{
    if(renderMutex_)
    {
        renderMutex_.unlock();
        renderMutex_.release();
    }
}

void Editor::makeCurrentContext()
{
    if(game_)
    {
        renderMutex_ = std::unique_lock<std::mutex>(game_->getRendererMutex());
    }

    SDL_GL_MakeCurrent(window_, glContext_);
}

void Editor::shutdown()
{
    releaseCurrentContext();

    if(game_)
        game_->shutdown();

    if(gameThread_.valid())
        gameThread_.wait();

    // Ensure we don't use the gl context of game to be able to delete it
    SDL_GL_MakeCurrent(window_, glContext_);
    SDL_GL_DeleteContext(gameGlContext_);
}

GameEngine::GameRef Editor::game() const
{
    return game_;
}
