//
// Created by adria on 16/10/2019.
//

#include <imgui.h>
#include "imgui_backend/imgui_stdlib.h"
#include <tinyfiledialogs.h>
#include "CreateProjectEditor.hpp"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

CreateProjectEditor::CreateProjectEditor(const CreateProjectEditor::ConfirmCallback &callback) :
    opened_(false),
    canCreateProject_(false),
    confirmCallback_(callback)
{
}

void CreateProjectEditor::Render()
{
    if(!opened_)
        return;

    const char *popupName = "New Project";
    if(!ImGui::IsPopupOpen(popupName))
        ImGui::OpenPopup(popupName);

    if (ImGui::BeginPopupModal(popupName))
    {
        if(ImGui::InputText("Project Root Folder##projectFolder", &newProject_->folderPath_))
            computeCanCreateProject();

        if (ImGui::Button("Select Root..."))
        {
            char const *folderProjectName = tinyfd_selectFolderDialog("Root Folder", newProject_->folderPath_.c_str());
            if (folderProjectName)
            {
                newProject_->folderPath_ = folderProjectName;
                computeCanCreateProject();
            }
        }

        if(ImGui::InputText("Project Name", &newProject_->folderName_))
            computeCanCreateProject();

        if(!canCreateProject_)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
            ImGui::Button("Create");
            ImGui::PopStyleVar();
        }
        else
        {
            if (ImGui::Button("Create"))
                confirm();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            close();

        ImGui::EndPopup();
    }
}

void CreateProjectEditor::open()
{
    opened_ = true;
    newProject_ = std::make_shared<ProjectData>();
}

bool CreateProjectEditor::isOpen() const
{
    return opened_;
}

void CreateProjectEditor::close()
{
    opened_ = false;
    ImGui::CloseCurrentPopup();
}

void CreateProjectEditor::confirm()
{
    fs::path folder(newProject_->folderPath_);
    folder.append(newProject_->folderName_);

    fs::path assetFolder(folder);
    assetFolder.append("assets");

    fs::path confFolder(folder);
    confFolder.append("conf");

    fs::path dataFolder(folder);
    dataFolder.append("data");

    fs::create_directories(assetFolder);
    fs::create_directories(confFolder);
    fs::create_directories(dataFolder);

    YAML::Node projectNode;
    projectNode = *newProject_.get();
    std::ofstream projectFile;
    projectFile.open(folder.append(newProject_->folderName_ + ".project").string());
    projectFile << projectNode << std::endl;
    projectFile.close();

    if(confirmCallback_)
        confirmCallback_(newProject_);

    close();
}

void CreateProjectEditor::computeCanCreateProject()
{
    canCreateProject_ = !newProject_->folderPath_.empty();

    if(canCreateProject_)
    {
        fs::path folder(newProject_->folderPath_);
        canCreateProject_ = fs::exists(folder);

        if (canCreateProject_)
        {
            folder.append(newProject_->folderName_);
            canCreateProject_ = !fs::exists(folder);
        }
    }
}
