//
// Created by adria on 26/01/2020.
//

#include <boost/filesystem.hpp>
#include <imgui.h>
#include "../imgui_backend/imgui_stdlib.h"
#include "CreatePrototypeDialog.h"

namespace fs = boost::filesystem;

CreatePrototypeDialog::CreatePrototypeDialog(const BaseDialog<boost::filesystem::path>::ConfirmCallback &callback) :
BaseDialog(callback)
{
}

void CreatePrototypeDialog::renderContent()
{
    if(ImGui::InputText("Prototype name", &name_))
        updateData();
}

bool CreatePrototypeDialog::canConfirm()
{
    return fs::portable_file_name(name_) && !fs::exists(data_);
}

void CreatePrototypeDialog::open(const boost::filesystem::path &basePath, const std::string &defaultName)
{
    basePath_ = basePath;
    BaseDialog::open(defaultName);
    name_ = defaultName;
    updateData();
}

void CreatePrototypeDialog::updateData()
{
    data_= basePath_ / (name_ + ".prototype");
}
