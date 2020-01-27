//
// Created by adria on 31/12/2019.
//

#include "ProjectPrototypeProvider.h"

ObjectDataRef ProjectPrototypeProvider::getPrototype(const std::string &filepath)
{
    ObjectDataRef prototype;
    auto it = _objectsLoadedCache.find(filepath);
    if(it != _objectsLoadedCache.end())
    {
        prototype = it->second;
    }
    else
    {
        YAML::Node prototypeNode = YAML::LoadFile(filepath);
        prototype = std::make_shared<ObjectData>(prototypeNode.as<ObjectData>());
        _objectsLoadedCache[filepath] = prototype;
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
