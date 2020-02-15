//
// Created by adria on 03/02/2020.
//

#ifndef GAMEDEVWARS_DATAFILE_H
#define GAMEDEVWARS_DATAFILE_H

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
    explicit DataFile(boost::filesystem::path filePath);

    DataFileType getType() const;

    const boost::filesystem::path &getFilePath() const;

    bool operator==(const DataFile &rhs) const;

    bool operator!=(const DataFile &rhs) const;
};



#endif //GAMEDEVWARS_DATAFILE_H
