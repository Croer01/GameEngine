//
// Created by adria on 31/12/2019.
//

#ifndef GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H
#define GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H


#include <map>
#include "ViewModels.hpp"

class ProjectFileDataProvider
{
    std::map<std::string, ObjectDataRef> objectsLoadedCache_;
    std::map<std::string, SceneDataRef> scenesLoadedCache_;
    ProjectDataRef project_;
public:
    ObjectDataRef getObjectData(const DataFile &file);
    SceneDataRef getSceneData(const DataFile &file);
    bool deleteData(const std::string &filepath);
    void clearCache();
    void setProject(const ProjectDataRef &projectData);
};

#endif //GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H
