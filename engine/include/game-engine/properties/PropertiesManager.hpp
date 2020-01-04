//
// Created by adria on 28/10/2019.
//

#ifndef GAMEENGINEEDITOR_PROPERTIESMANAGER_HPP
#define GAMEENGINEEDITOR_PROPERTIESMANAGER_HPP

#include <map>
#include <game-engine/properties/PropertySet.hpp>
#include "../../../private/Singleton.hpp"

#define PROPERTIES(x) //the #x properties set has been registered

namespace GameEngine
{

class PropertyInstantiator
{
    std::shared_ptr<PropertySetBase> properties_;
public:
    std::shared_ptr<PropertySetBase> create()
    {
        if(!properties_)
        {
            PropertySetBase *instance = instantiateProperties();
            properties_.reset(instance);
        }
        return properties_;
    };

    virtual PropertySetBase *instantiateProperties() = 0;
};

}
#endif //GAMEENGINEEDITOR_PROPERTIESMANAGER_HPP
