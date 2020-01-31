//
// Created by adria on 29/01/2020.
//

#ifndef GAMEDEVWARS_PROJECTDIRECTORY_H
#define GAMEDEVWARS_PROJECTDIRECTORY_H

#include <boost/filesystem/path.hpp>

enum class DataFileType
{
    Other,
    Prototype,
    Scene,
};

class DataFile;
typedef std::shared_ptr<DataFile> DataFileRef;
class DataFile
{
    DataFileType type_;
    boost::filesystem::path filePath_;

public:
    explicit DataFile(const boost::filesystem::path &filePath);

    DataFileType getType() const;

    const boost::filesystem::path &getFilePath() const;
};

class ProjectDirectory
{

    // TODO: refactor this as a tree strucuture
    std::vector<DataFile> files_;
};


#endif //GAMEDEVWARS_PROJECTDIRECTORY_H
