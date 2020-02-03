//
// Created by adria on 27/01/2020.
//

#include <imgui.h>
#include "DeleteFileDialog.h"

DeleteFileDialog::DeleteFileDialog(const BaseDialog<boost::filesystem::path>::ConfirmCallback &callback) : BaseDialog<boost::filesystem::path>(callback)
{}

void DeleteFileDialog::renderContent()
{
    ImGui::Text("Are you sure to delete %ls ?", data_.c_str());
}
