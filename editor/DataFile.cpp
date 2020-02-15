//
// Created by adria on 03/02/2020.
//

#include "DataFile.h"

#include <utility>

DataFile::DataFile():
        filePath_(), type_(DataFileType::Other)
{
}

DataFile::DataFile(boost::filesystem::path filePath) :
        filePath_(std::move(filePath))
{
    if (filePath_.extension() == ".prototype")
        type_ = DataFileType::Prototype;
    else if (filePath_.extension() == ".scene")
        type_ = DataFileType::Scene;
    else if (filePath_.extension() == ".png")
        type_ = DataFileType::Image;
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

bool DataFile::operator==(const DataFile &rhs) const
{
    return type_ == rhs.type_ &&
           filePath_ == rhs.filePath_;
}

bool DataFile::operator!=(const DataFile &rhs) const
{
    return !(rhs == *this);
}
