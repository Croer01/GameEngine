//
// Created by adria on 31/12/2019.
//

#include "ProjectFileDataProvider.h"

ObjectDataRef ProjectFileDataProvider::getObjectData(const DataFile &file)
{
    if(file.getType() != DataFileType::Prototype)
        throw std::invalid_argument("the file " + file.getFilePath().string() + " is not a Prototype");

    ObjectDataRef prototype;
    const std::string &filepath = file.getFilePath().string();
    auto it = _objectsLoadedCache.find(filepath);
    if(it != _objectsLoadedCache.end())
    {
        prototype = it->second;
    }
    else
    {
        try
        {
            YAML::Node prototypeNode = YAML::LoadFile(filepath);
            prototype = std::make_shared<ObjectData>(prototypeNode.as<ObjectData>());
            _objectsLoadedCache[filepath] = prototype;

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
    const std::string &filepath = file.getFilePath().string();
    auto it = _scenesLoadedCache.find(filepath);
    if(it != _scenesLoadedCache.end())
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
            _scenesLoadedCache[filepath] = scene;

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
        auto it = _objectsLoadedCache.find(filepath);
        if (it != _objectsLoadedCache.end())
        {
            deleted = true;
            _objectsLoadedCache.erase(it);
        }
    }
    else if(dataFile.getType() == DataFileType::Scene)
    {
        auto it = _scenesLoadedCache.find(filepath);
        if (it != _scenesLoadedCache.end())
        {
            deleted = true;
            _scenesLoadedCache.erase(it);
        }
    }
    else {
        throw new std::invalid_argument("the file " + filepath + " doesn't have data loaded or it is an unsuported type");
    }

    return deleted;
}

void ProjectFileDataProvider::clearCache()
{
    _objectsLoadedCache.clear();
    _scenesLoadedCache.clear();
}
