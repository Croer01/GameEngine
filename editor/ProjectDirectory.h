//
// Created by adria on 29/01/2020.
//

#ifndef GAMEDEVWARS_PROJECTDIRECTORY_H
#define GAMEDEVWARS_PROJECTDIRECTORY_H

#include <boost/filesystem/path.hpp>
#include "ViewModels.hpp"
#include "DataFile.h"
#include "DataDirectory.hpp"

class ProjectDirectory
{
    // TODO: refactor this as a tree structure
    std::vector<DataFile> files_;
    DataDirectoryRef directory_;
    ProjectDataRef project_;
    std::vector<DataFile> editedFiles_;

    void recursiveDataFilesRegister(const boost::filesystem::path &directoryPath, const DataDirectoryRef &directory);
public:
    explicit ProjectDirectory(const ProjectDataRef &project);
    void markEdited(const DataFile &fileEdited);
    void addFile(const boost::filesystem::path &filePath);
    void removeFile(const boost::filesystem::path &filePath);
    bool hasEditedFiles() const;
    void markAllSaved();

    std::vector<DataFile> getFiles() const;
    std::vector<DataFile> getFiles(DataFileType fileType) const;

    std::vector<DataFile> getEditedFiles() const;
    DataDirectoryRef getTree() const;
};


#endif //GAMEDEVWARS_PROJECTDIRECTORY_H
