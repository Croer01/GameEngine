//
// Created by adria on 31/01/2021.
//

#ifndef GAMEEDITOR_DATAFILE_HPP
#define GAMEEDITOR_DATAFILE_HPP

#include <boost/filesystem/path.hpp>

enum class DataFileType
{
    Other,
    Prototype,
    Scene,
    Image
};

class DataFile;
typedef std::shared_ptr<DataFile> DataFileRef;
class DataFile
{
    DataFileType type_;
    boost::filesystem::path filePath_;

public:
    DataFile();
    explicit DataFile(const boost::filesystem::path &filePath);
    DataFileType getType() const;
    boost::filesystem::path getFullPath() const;
    std::string DataFile::getName() const;

    bool operator==(const DataFile &rhs) const;
    bool operator!=(const DataFile &rhs) const;
};



#endif //GAMEEDITOR_DATAFILE_HPP
