//
// Created by adria on 29/01/2020.
//

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "ProjectDirectory.h"

namespace fs = boost::filesystem;

void ProjectDirectory::markEdited(const DataFile &fileEdited)
{
    if(std::find(editedFiles_.begin(),editedFiles_.end(), fileEdited) == editedFiles_.end())
    {
        editedFiles_.push_back(fileEdited);
    }
}

bool ProjectDirectory::hasEditedFiles() const
{
    return !editedFiles_.empty();
}

void ProjectDirectory::markAllSaved()
{
    editedFiles_.clear();
}

ProjectDirectory::ProjectDirectory(const ProjectDataRef &project)
{
    project_ = project;

    // List the data files
    // TODO: observe directory to update list if some file is created/deleted
    // probably  Implement in a separate thread
    root_ = std::make_shared<DataDirectory>(project->dataPath_);
    recursiveDataFilesRegister(project->dataPath_);
}

void ProjectDirectory::recursiveDataFilesRegister(const boost::filesystem::path &directoryPath)
{
    if(fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        fs::directory_iterator end;
        for (fs::directory_iterator itr(directoryPath); itr != end; ++itr)
        {
            //TODO: list all files types (create Metadata object)
            if (is_regular_file(itr->path()))
            {
                files_.emplace_back(itr->path());
                // Make file path relative to the data directory to work with the other Editor components
                // TODO: Should Editor unify all the path to be relative to the project directory instead of data?
                root_->addFile(std::make_shared<DataFile>(fs::relative(itr->path(), project_->dataPath_)));
            }
            else
            {
                // filename in this case is the last directory (based on boost::filesystem docs)
                auto dir = std::make_shared<DataDirectory>(fs::relative(itr->path(), project_->dataPath_));
                root_->addFolder(dir);
                recursiveDataFilesRegister(itr->path());
            }
        }
    }
}

std::vector<DataFile> ProjectDirectory::getFiles() const
{
    return files_;
}

std::vector<DataFile> ProjectDirectory::getFiles(DataFileType fileType) const
{
    std::vector<DataFile> itemsFiltered;
    std::copy_if (files_.begin(), files_.end(), std::back_inserter(itemsFiltered),
            [fileType](const DataFile& file){return file.getType() == fileType;} );
    return itemsFiltered;
}

std::vector<DataFile> ProjectDirectory::getEditedFiles() const
{
    return editedFiles_;
}

void ProjectDirectory::addFile(const boost::filesystem::path &filePath)
{
    root_->addFile(std::make_shared<DataFile>(fs::relative(filePath, project_->dataPath_)));
    files_.emplace_back(filePath);
}

void ProjectDirectory::removeFile(const boost::filesystem::path &filePath)
{
    root_->removeFile(fs::relative(filePath, project_->dataPath_));
    //TODO: check if the file to delete is edited?
    auto it = std::remove_if( files_.begin(),
                              files_.end(),
                              [filePath](const DataFile &file){ return file.getFilePath() == filePath; } );

    assert(it != files_.end());
    files_.erase(it, files_.end());
}

DataDirectoryRef ProjectDirectory::getTree() const
{
    return root_;
}
