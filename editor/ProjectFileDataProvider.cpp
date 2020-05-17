//
// Created by adria on 31/12/2019.
//

#include <boost/filesystem/operations.hpp>
#include "ProjectFileDataProvider.h"

namespace fs = boost::filesystem;
ObjectDataRef ProjectFileDataProvider::getObjectData(const DataFile &file)
{
    if(file.getType() != DataFileType::Prototype)
        throw std::invalid_argument("the file " + file.getFilePath().string() + " is not a Prototype");

    ObjectDataRef prototype;
    const std::string &filepath = fs::absolute(file.getFilePath(), project_->dataPath_).string();
    auto it = objectsLoadedCache_.find(filepath);
    if(it != objectsLoadedCache_.end())
    {
        prototype = it->second;
    }
    else
    {
        try
        {
            YAML::Node prototypeNode = YAML::LoadFile(filepath);
            prototype = std::make_shared<ObjectData>(prototypeNode.as<ObjectData>());
            objectsLoadedCache_[filepath] = prototype;

        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error loading " + filepath + ":\n" + e.what());
        }

    }

    return prototype;
}

SceneDataRef ProjectFileDataProvider::getSceneData(const DataFile &file)
{
    if(file.getType() != DataFileType::Scene)
        throw std::invalid_argument("the file " + file.getFilePath().string() + " is not a Scene");

    SceneDataRef scene;
    const std::string &filepath = fs::absolute(file.getFilePath(), project_->dataPath_).string();
    auto it = scenesLoadedCache_.find(filepath);
    if(it != scenesLoadedCache_.end())
    {
        scene = it->second;
    }
    else
    {
        try
        {
            YAML::Node prototypeNode = YAML::LoadFile(filepath);
            scene = std::make_shared<SceneData>(prototypeNode.as<SceneData>());
            scene->filePath_ = file.getFilePath().string();
            scenesLoadedCache_[filepath] = scene;

        }
        catch (const std::exception &e)
        {
            throw std::runtime_error("Error loading " + filepath + ":\n" + e.what());
        }

    }

    return scene;
}

bool ProjectFileDataProvider::deleteData(const std::string &filepath)
{
    DataFile dataFile(filepath);
    bool deleted = false;

    if(dataFile.getType() == DataFileType::Prototype)
    {
        auto it = objectsLoadedCache_.find(filepath);
        if (it != objectsLoadedCache_.end())
        {
            deleted = true;
            objectsLoadedCache_.erase(it);
        }
    }
    else if(dataFile.getType() == DataFileType::Scene)
    {
        auto it = scenesLoadedCache_.find(filepath);
        if (it != scenesLoadedCache_.end())
        {
            deleted = true;
            scenesLoadedCache_.erase(it);
        }
    }
    else {
        throw new std::invalid_argument("the file " + filepath + " doesn't have data loaded or it is an unsuported type");
    }

    return deleted;
}

void ProjectFileDataProvider::clearCache()
{
    objectsLoadedCache_.clear();
    scenesLoadedCache_.clear();
}

void ProjectFileDataProvider::setProject(const ProjectDataRef &projectData)
{
    project_ = projectData;
}
