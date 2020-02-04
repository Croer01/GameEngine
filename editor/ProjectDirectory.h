//
// Created by adria on 29/01/2020.
//

#ifndef GAMEDEVWARS_PROJECTDIRECTORY_H
#define GAMEDEVWARS_PROJECTDIRECTORY_H

#include <boost/filesystem/path.hpp>
#include "ViewModels.hpp"
#include "DataFile.h"

class ProjectDirectory
{
    // TODO: refactor this as a tree strucuture
    std::vector<DataFile> files_;

    std::vector<DataFile> editedFiles_;

public:
    explicit ProjectDirectory(const ProjectDataRef &project);
    void markEdited(const DataFile &fileEdited);
    void addFile(const boost::filesystem::path &filePath);
    void removeFile(const boost::filesystem::path &filePath);
    bool hasEditedFiles() const;
    void markAllSaved();

    const std::vector<DataFile> &getFiles() const;

    const std::vector<DataFile> &getEditedFiles() const;
};


#endif //GAMEDEVWARS_PROJECTDIRECTORY_H
