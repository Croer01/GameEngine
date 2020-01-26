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
        BaseDialog<ProjectDataRef>(callback),
        canCreateProject_(false)
{
    dialogName_ = "Create Project";
    confirmButtonName_ = "Create";
}

void CreateProjectEditor::renderContent()
{
    if(ImGui::InputText("Project Root Folder##projectFolder", &data_->folderPath_))
        computeCanCreateProject();

    if (ImGui::Button("Select Root..."))
    {
        char const *folderProjectName = tinyfd_selectFolderDialog("Root Folder", data_->folderPath_.c_str());
        if (folderProjectName)
        {
            data_->folderPath_ = folderProjectName;
            computeCanCreateProject();
        }
    }

    if(ImGui::InputText("Project Name", &data_->folderName_))
        computeCanCreateProject();
}

void CreateProjectEditor::onOpen()
{
    data_ = std::make_shared<ProjectData>();
}

void CreateProjectEditor::onConfirm()
{
    fs::path folder(data_->folderPath_);
    folder.append(data_->folderName_);

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
    projectNode = *data_;
    std::ofstream projectFile;
    projectFile.open(folder.append(data_->folderName_ + ".project").string());
    projectFile << projectNode << std::endl;
    projectFile.close();
}

void CreateProjectEditor::computeCanCreateProject()
{
    canCreateProject_ = !data_->folderPath_.empty();

    if(canCreateProject_)
    {
        fs::path folder(data_->folderPath_);
        canCreateProject_ = fs::exists(folder);

        if (canCreateProject_)
        {
            folder.append(data_->folderName_);
            canCreateProject_ = !fs::exists(folder);
        }
    }
}

bool CreateProjectEditor::canConfirm()
{
    return canCreateProject_;
}
