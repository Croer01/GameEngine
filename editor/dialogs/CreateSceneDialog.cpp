//
// Created by adria on 02/05/2020.
//

#include <boost/filesystem.hpp>
#include <imgui.h>
#include "../imgui_backend/imgui_stdlib.h"
#include "CreateSceneDialog.h"

namespace fs = boost::filesystem;

CreateSceneDialog::CreateSceneDialog(const BaseDialog<std::string>::ConfirmCallback &callback) :
BaseDialog(callback)
{
}

void CreateSceneDialog::renderContent()
{
    ImGui::InputText("Scene name", &data_);
}

bool CreateSceneDialog::canConfirm()
{
    return fs::portable_file_name(data_);
}
