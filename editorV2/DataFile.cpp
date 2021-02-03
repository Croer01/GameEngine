//
// Created by adria on 03/02/2020.
//

#include "DataFile.hpp"

DataFile::DataFile():
        filePath_(), type_(DataFileType::Other)
{
}

DataFile::DataFile(const boost::filesystem::path &filePath) :
        filePath_(filePath)
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

boost::filesystem::path DataFile::getFullPath() const
{
    return filePath_;
}

std::string DataFile::getName() const
{
    // filename in this case is the last directory, based on boost::filesystem docs
    return filePath_.filename().string();
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
