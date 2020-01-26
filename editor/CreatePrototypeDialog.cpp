//
// Created by adria on 26/01/2020.
//

#include <boost/filesystem.hpp>
#include <imgui.h>
#include "imgui_backend/imgui_stdlib.h"
#include "CreatePrototypeDialog.h"

namespace fs = boost::filesystem;

CreatePrototypeDialog::CreatePrototypeDialog(const BaseDialog<std::string>::ConfirmCallback &callback) :
BaseDialog(callback)
{
}

void CreatePrototypeDialog::renderContent()
{
    ImGui::InputText("Prototype name", &data_);
}

bool CreatePrototypeDialog::canConfirm()
{
    return fs::portable_file_name(data_);
}

void CreatePrototypeDialog::open(const std::string &defaultValue)
{
    data_ = defaultValue.empty();
    BaseDialog::open();
}
