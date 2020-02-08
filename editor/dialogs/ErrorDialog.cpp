//
// Created by adria on 08/02/2020.
//

#include <imgui.h>
#include "ErrorDialog.h"

void ErrorDialog::renderContent()
{
    ImGui::Text("Unexpected error: %s", data_.c_str());
}

ErrorDialog::ErrorDialog() : BaseDialog(nullptr)
{
    dialogName_ = "Error";
}
