//
// Created by adria on 16/05/2020.
//

#include <iostream>
#include "DataDirectory.hpp"

namespace fs = boost::filesystem;

DataDirectory::DataDirectory(const boost::filesystem::path &directoryPath) :
    directoryPath_(directoryPath)
{
}

std::string DataDirectory::name() const
{
    // filename in this case is the last directory, based on boost::filesystem docs
    return directoryPath_.filename().string();
}

std::vector<DataDirectoryRef> DataDirectory::getFolders() const
{
    return subDirectories_;
}

std::vector<DataFileRef> DataDirectory::getFiles() const
{
    return files_;
}

void DataDirectory::addFile(const DataFileRef &file)
{
    DataDirectory *target = getDirectory(file->getFilePath());
    target->files_.push_back(file);
}

DataDirectory *DataDirectory::getDirectory(const boost::filesystem::path &filePath)
{
    DataDirectory *target = this;
    if(filePath.has_parent_path())
    {
        for (const auto &part : filePath.parent_path())
        {
            std::cout << part.string() << std::endl;
            auto it = std::find_if(target->subDirectories_.begin(), target->subDirectories_.end(),
                                   [part](const DataDirectoryRef &directory)
                                   {
                                       return directory->name() == part.string();
                                   });
            assert(it != target->subDirectories_.end());
            target = (*it).get();
        }
    }

    return target;
}

void DataDirectory::addFolder(const DataDirectoryRef &folder)
{
    DataDirectory *target = getDirectory(folder->getDirectoryPath());
    target->subDirectories_.push_back(folder);
}

void DataDirectory::removeFile(const boost::filesystem::path &filePath)
{
    DataDirectory *target = getDirectory(filePath);

    auto it = std::find_if(target->files_.begin(), target->files_.end(), [filePath](const DataFileRef &file)
    {
        return file->getFilePath().filename() == filePath.filename();
    });
    assert(it != target->files_.end());
    target->files_.erase(it);
}

const boost::filesystem::path &DataDirectory::getDirectoryPath() const
{
    return directoryPath_;
}
