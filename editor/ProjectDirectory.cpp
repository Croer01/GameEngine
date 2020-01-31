//
// Created by adria on 29/01/2020.
//

#include "ProjectDirectory.h"

DataFile::DataFile(const boost::filesystem::path &filePath) :
        filePath_(filePath)
{
    if (filePath_.extension() == ".prototype")
        type_ = DataFileType::Prototype;
    if (filePath_.extension() == ".scene")
        type_ = DataFileType::Scene;
    else
        type_ = DataFileType::Other;
}

DataFileType DataFile::getType() const
{
    return type_;
}

const boost::filesystem::path &DataFile::getFilePath() const
{
    return filePath_;
}
