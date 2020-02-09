//
// Created by adria on 31/12/2019.
//

#include "ProjectPrototypeProvider.h"

ObjectDataRef ProjectPrototypeProvider::getPrototype(const DataFile &file)
{
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

ObjectDataRef ProjectPrototypeProvider::deletePrototype(const std::string &filepath)
{
    ObjectDataRef objectDeleted;

    auto it = _objectsLoadedCache.find(filepath);
    if(it != _objectsLoadedCache.end()) {
        objectDeleted = it->second;
        _objectsLoadedCache.erase(it);
    }

    return objectDeleted;
}

void ProjectPrototypeProvider::clearCache()
{
    _objectsLoadedCache.clear();
}
