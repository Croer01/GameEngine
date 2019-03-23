//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/geData.hpp>
#include <game-engine/geComponent.hpp>
#include <memory>

namespace GameEngine {
    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual geComponentRef Create(const geData &data) = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual geComponentRef Create(const geData &data) {

            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            PropertySet<ComponentType> &properties = static_cast<PropertySet<ComponentType>&>(instance->properties());

            for(int i = 0; i < properties.size(); i++){
                PropertyBase<ComponentType> &property = properties.get(i);

                if(!data.hasValue(property.name())) {
                    if(property.required())
                        throw std::logic_error("property " + property.name() + " is required but it isn't defined");
                    continue;
                }

                switch (property.type()){
                    case PropertyTypes::INT:
                        {
                            auto propertyInt = static_cast<Property<ComponentType,int>&>(property);
                            propertyInt.set(data.getInt(property.name()));
                        }
                        break;
                    case PropertyTypes::FLOAT:
                        {
                            auto propertyFloat = static_cast<Property<ComponentType,float>&>(property);
                            propertyFloat.set(data.getFloat(property.name()));
                        }
                        break;
                    case PropertyTypes::STRING:
                        {
                            auto propertyString = static_cast<Property<ComponentType,std::string>&>(property);
                            propertyString.set(data.getString(property.name()));
                        }
                        break;
                    case PropertyTypes::VEC2D:
                        {
                            auto propertyVec2d = static_cast<Property<ComponentType,Vec2D>&>(property);
                            propertyVec2d.set(data.getVec2D(property.name()));
                        }
                        break;

                    case PropertyTypes::BOOL:
                        {
                            auto propertyBool = static_cast<Property<ComponentType, bool>&>(property);
                            propertyBool.set(data.getBool(property.name()));
                        }
                        break;
                    case PropertyTypes::ARRAY_STRING:
                        {
                            auto propertyArray = static_cast<Property<ComponentType, std::vector<std::string>>&>(property);
                            propertyArray.set(data.getArrayString(property.name()));
                        }
                        break;
                    case PropertyTypes::UNKNOWN:
                        throw std::runtime_error("the property " + property.name() + " has unknown type");
                        break;
                }
            }

            return instance;
        };
    };
}

#endif //SPACEINVADERS_BUILDER_HPP
