//
// Created by adria on 31/01/2021.
//

#ifndef GAMEEDITOR_DATADIRECTORY_HPP
#define GAMEEDITOR_DATADIRECTORY_HPP


#include <vector>
#include <memory>
#include "DataFile.hpp"

class DataDirectory;

typedef std::shared_ptr<DataDirectory> DataDirectoryRef;

class DataDirectory
{
    boost::filesystem::path directoryPath_;
    std::vector<DataDirectoryRef> subDirectories_;
    std::vector<DataFileRef> files_;
    DataDirectory *getSubDirectory(const boost::filesystem::path &directory);
    void internalFilterFilesByType(DataFileType fileType, std::vector<DataFileRef> &filteredFiles) const;

public:
    explicit DataDirectory(const boost::filesystem::path &directoryPath);
    void addElement(const boost::filesystem::path &path);
    std::string getName() const;
    boost::filesystem::path getFullPath() const;
    std::vector<DataDirectoryRef> getFolders() const;
    std::vector<DataFileRef> getFiles() const;
    std::vector<DataFileRef> filterFilesByType(DataFileType fileType) const;
};


#endif //GAMEEDITOR_DATADIRECTORY_HPP
