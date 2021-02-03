//
// Created by adria on 28/01/2021.
//

#include <imgui.h>
#include <boost/filesystem.hpp>
#include "FileExplorer.hpp"

namespace fs = boost::filesystem;

void FileExplorer::setRoot(const boost::filesystem::path &directory)
{
    root_ = std::make_shared<DataDirectory>(directory);
    recursiveFilesRegister(directory);
}

void FileExplorer::update()
{
    if(!root_)
        return;

    ImGui::Begin("File explorer");
    ImGui::Text("Directory: %s", root_->getFullPath().string().c_str());

    renderFileListInternal(root_);

    ImGui::End();
}

void FileExplorer::renderFileListInternal(const DataDirectoryRef &dir)
{
    ImGui::PushID(dir->getName().c_str());
    for(const auto &file : dir->getFiles())
    {
        std::string filename = file->getName();
        ImGui::PushID(filename.c_str());
        ImGuiTreeNodeFlags PrototypesNodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
        ImGui::TreeNodeEx(filename.c_str(), PrototypesNodeFlags);
        if(ImGui::IsItemClicked())
        {
            DataFile &dataFile = *file.get();
            switch (file->getType())
            {
                case DataFileType::Prototype:
                    // TODO
                    break;
                case DataFileType::Scene:
                    // TODO
                    break;
            }
        }
        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 3);
        if(ImGui::Button("X"))
        {
            // TODO
        }
        ImGui::PopID();
    }
    ImGui::PopID();

    ImGui::PushID("dirs");
    for(const auto &subdir : dir->getFolders())
    {
        ImGui::PushID(subdir->getName().c_str());
        bool opened = ImGui::TreeNode(subdir->getName().c_str());

        float labelWidth = ImGui::CalcTextSize("Create...").x;
        ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - labelWidth);
        if(ImGui::Button("Create..."))
        {
            // TODO
        }

        if(opened)
        {
            renderFileListInternal(subdir);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::PopID();
}

void FileExplorer::recursiveFilesRegister(const boost::filesystem::path &directoryPath)
{
    if(fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        fs::directory_iterator end;
        for (fs::directory_iterator itr(directoryPath); itr != end; ++itr)
        {
            if (is_regular_file(itr->path()))
            {
                root_->addElement(itr->path());
            }
            else
            {
                // filename in this case is the last directory (based on boost::filesystem docs)
                root_->addElement(itr->path());
                recursiveFilesRegister(itr->path());
            }
        }
    }
}
