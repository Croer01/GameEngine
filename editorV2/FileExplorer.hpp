//
// Created by adria on 28/01/2021.
//

#ifndef GAMEEDITOR_FILEEXPLORER_HPP
#define GAMEEDITOR_FILEEXPLORER_HPP


#include <memory>
#include "DataDirectory.hpp"

class FileExplorer
{
    DataDirectoryRef root_;
    void renderFileListInternal(const DataDirectoryRef &dir);
    void recursiveFilesRegister(const boost::filesystem::path &directoryPath);
public:
    void setRoot(const boost::filesystem::path &directory);
    void update();
};


#endif //GAMEEDITOR_FILEEXPLORER_HPP
