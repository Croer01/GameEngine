//
// Created by adria on 26/01/2020.
//

#include <boost/filesystem.hpp>
#include <imgui.h>
#include "../imgui_backend/imgui_stdlib.h"
#include "CreateDataFileDialog.h"

namespace fs = boost::filesystem;

CreateDataFileDialog::CreateDataFileDialog(const BaseDialog<boost::filesystem::path>::ConfirmCallback &callback) :
    BaseDialog(callback),
    fileTypes_({ "Prototype", "Scene" }),
    selectedFileType_(0)
{
    dialogName_ = "Create data object";
    confirmButtonName_ = "Create";
}

void CreateDataFileDialog::renderContent()
{
    ImGui::Text("Base directory: %s", basePath_.string().c_str());

    if(ImGui::Combo("type", &selectedFileType_, fileTypes_.data(), fileTypes_.size()))
        updateData();

    std::string inputLabel = std::string(fileTypes_[selectedFileType_]) + " name";
    if(ImGui::InputText(inputLabel.c_str(), &name_))
        updateData();
}

bool CreateDataFileDialog::canConfirm()
{
    assert(!fs::is_other(dataPath_));
    return fs::portable_file_name(name_) && !fs::exists(dataPath_ / data_);
}

void CreateDataFileDialog::open(const boost::filesystem::path &basePath, const std::string &defaultName)
{
    basePath_ = basePath;
    BaseDialog::open(defaultName);
    name_ = defaultName;
    updateData();
}

void CreateDataFileDialog::updateData()
{
    std::string fileType = selectedFileType_ == 0? ".prototype" : ".scene";
    data_= basePath_ / (name_ + fileType);
}

void CreateDataFileDialog::setDataPath(const boost::filesystem::path &dataPath)
{
    dataPath_ = dataPath;
}
