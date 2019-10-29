//
// Created by adria on 28/10/2019.
//

#ifndef GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
#define GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP

#include <game-engine/properties/PropertySet.hpp>

namespace GameEngine
{

template <typename Target>
struct PropertyInstantiator
{
    static PropertySetBase *instantiate()
    {
        throw std::runtime_error("PropertyInstantiatior not implemented for type " + std::type_info<Target>().name());
    }
};

}
#endif //GAMEENGINEEDITOR_PROPERTIESREGISTER_HPP
