//
// Created by adria on 03/02/2020.
//

#include <imgui.h>
#include "SaveAllDialog.h"

SaveAllDialog::SaveAllDialog(const BaseDialog<int>::ConfirmCallback &callback) : BaseDialog<int>(callback)
{
    dialogName_ = "Save All files";
    confirmButtonName_ = "Save All";
}

void SaveAllDialog::renderContent()
{
    ImGui::Text("There are files modified. Save all?");

    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
    if (ImGui::ListBoxHeader("##", filesToSave_.size()))
    {
        for(const auto &file : filesToSave_)
        {
            ImGui::Text("%s", file.getFilePath().string().c_str());
        }
        ImGui::ListBoxFooter();
    }
}

void SaveAllDialog::setFilesToSave(const std::vector<DataFile> &files)
{
    filesToSave_ = files;
}
