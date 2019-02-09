//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/geComponent.hpp>
#include <memory>
namespace GameEngine {
namespace Internal {
    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual geComponentRef Create(const YAML::Node &data) = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual geComponentRef Create(const YAML::Node &data) {

            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            PropertySet<ComponentType> &properties = static_cast<PropertySet<ComponentType>&>(instance->properties());

            for(int i = 0; i < properties.size(); i++){
                PropertyBase<ComponentType> &property = properties.get(i);

                if(!data[property.name()]) {
                    if(property.required())
                        throw std::logic_error("property " + property.name() + " is required but it isn't defined");
                    continue;
                }

                switch (property.type()){
                    case PropertyTypes::INT:
                        {
                            auto propertyInt = static_cast<Property<ComponentType,int>&>(property);
                            propertyInt.set(data[property.name()].as<int>(0));
                        }
                        break;
                    case PropertyTypes::FLOAT:
                        {
                            auto propertyFloat = static_cast<Property<ComponentType,float>&>(property);
                            propertyFloat.set(data[property.name()].as<float>(0.f));
                        }
                        break;
                    case PropertyTypes::STRING:
                        {
                            auto propertyString = static_cast<Property<ComponentType,std::string>&>(property);
                            propertyString.set(data[property.name()].as<std::string>(""));
                        }
                        break;
                    case PropertyTypes::VEC2D:
                        {
                            auto propertyVec2d = static_cast<Property<ComponentType,Vec2D>&>(property);
                            propertyVec2d.set({
                                data[property.name()][0].as<float>(0),
                                data[property.name()][1].as<float>(0)
                            });
                        }
                        break;

                    case PropertyTypes::BOOL:
                        {
                            auto propertyBool = static_cast<Property<ComponentType, bool>&>(property);
                            propertyBool.set(data[property.name()].as<bool>(false));
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
}

#endif //SPACEINVADERS_BUILDER_HPP
