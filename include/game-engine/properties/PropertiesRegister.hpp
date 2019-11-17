//
// Created by adria on 28/10/2019.
//

#ifndef GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
#define GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP

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

class PropertiesRegister : public Internal::Singleton<PropertiesRegister>
{
    std::map<std::string, std::shared_ptr<PropertyInstantiator>> properties_;

public:
    void addProperty(const std::string &targetName, const std::shared_ptr<PropertyInstantiator> &properties)
    {
        properties_[targetName] = properties;
    }

    std::shared_ptr<PropertySetBase> instantiate(const std::string &targetName)
    {
        return properties_[targetName]->create();
    }
};

}
#endif //GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
