//
// Created by adria on 31/12/2019.
//

#ifndef GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H
#define GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H


#include <map>
#include "ViewModels.hpp"

class ProjectPrototypeProvider
{
    std::map<std::string, ObjectDataRef> _objectsLoadedCache;
public:
    ObjectDataRef getPrototype(const std::string &filepath);
    void clearCache();
};


#endif //GAMEDEVWARS_PROJECTPROTOTYPEPROVIDER_H
