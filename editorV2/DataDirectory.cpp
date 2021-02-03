//
// Created by adria on 16/05/2020.
//

#include <iostream>
#include <boost/filesystem.hpp>
#include "DataDirectory.hpp"

namespace fs = boost::filesystem;

DataDirectory::DataDirectory(const boost::filesystem::path &directoryPath) :
    directoryPath_(directoryPath)
{
}

std::string DataDirectory::getName() const
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

void DataDirectory::addElement(const boost::filesystem::path &path)
{
    DataDirectory *target = getSubDirectory(path);

    if(fs::is_directory(path))
    {
        target->subDirectories_.push_back(std::make_shared<DataDirectory>(path));
    }
    else
    {
        target->files_.push_back(std::make_shared<DataFile>(path));
    }
}

DataDirectory *DataDirectory::getSubDirectory(const boost::filesystem::path &filePath)
{
    DataDirectory *target = this;

    fs::path relativePath = fs::relative(filePath, directoryPath_);

    if(relativePath.has_parent_path())
    {
        for (const auto &part : relativePath.parent_path())
        {
            std::cout << part.string() << std::endl;
            auto it = std::find_if(target->subDirectories_.begin(), target->subDirectories_.end(),
                                   [part](const DataDirectoryRef &directory)
                                   {
                                       return directory->getName() == part.string();
                                   });
            assert(it != target->subDirectories_.end());
            target = (*it).get();
        }
    }

    return target;
}

boost::filesystem::path DataDirectory::getFullPath() const
{
    return directoryPath_;
}
