//
// Created by adria on 28/10/2019.
//

#ifndef GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
#define GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP

#include <map>
#include <game-engine/properties/PropertySet.hpp>
#include "../../../private/Singleton.hpp"

namespace GameEngine
{

class PropertiesRegister : public Internal::Singleton<PropertiesRegister>
{
    std::map<std::string, std::shared_ptr<PropertySetBase>> properties_;

public:
    void addProperty(const std::string &targetName, const std::shared_ptr<PropertySetBase> &properties)
    {
        properties_[targetName] = properties;
    }
};

template <typename Target>
class PropertyInstantiator
{
    static PropertyInstantiator<Target> regitser_;
public:
    PropertyInstantiator()
    {
        PropertySetBase *propertiesSet = instantiate();
        auto propertiesSetRef = std::make_shared<PropertySetBase>(propertiesSet);
        PropertiesRegister::GetInstance().addProperty(getTargetName(), propertiesSetRef);
    }

    static std::string getTargetName()
    {
        throw std::runtime_error("PropertyInstantiatior not implemented for type " + std::type_info<Target>().name());
    }

    static PropertySetBase *instantiate()
    {
        throw std::runtime_error("PropertyInstantiatior not implemented for type " + std::type_info<Target>().name());
    }
};

template <typename Target>
PropertyInstantiator<Target> PropertyInstantiator<Target>::regitser_;

}
#endif //GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
