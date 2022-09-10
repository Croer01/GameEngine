//
// Created by adria on 16/05/2020.
//

#ifndef GAMEENGINE_DATADIRECTORY_HPP
#define GAMEENGINE_DATADIRECTORY_HPP


#include <vector>
#include <memory>
#include "DataFile.h"

class DataDirectory;

typedef std::shared_ptr<DataDirectory> DataDirectoryRef;

class DataDirectory
{
    boost::filesystem::path directoryPath_;
    std::vector<DataDirectoryRef> subDirectories_;
    std::vector<DataFileRef> files_;
    DataDirectory *getDirectory(const boost::filesystem::path &directory);

public:
    explicit DataDirectory(const boost::filesystem::path &directoryPath);
    const boost::filesystem::path &getDirectoryPath() const;
    std::string name() const;
    void addFile(const DataFileRef &file);
    void addFolder(const DataDirectoryRef &folder);
    std::vector<DataDirectoryRef> getFolders() const;
    std::vector<DataFileRef> getFiles() const;
    void removeFile(const boost::filesystem::path &filePath);
};


#endif //GAMEENGINE_DATADIRECTORY_HPP
