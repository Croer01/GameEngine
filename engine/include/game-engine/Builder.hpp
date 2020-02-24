//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/geData.hpp>
#include <memory>
#include <game-engine/properties/PropertySet.hpp>

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual geComponentRef Create(const geData &data) = 0;
        virtual std::shared_ptr<PropertySetBase> createProperties() = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual geComponentRef Create(const geData &data) {
            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            instance->copyProperties<ComponentType>(data);
            return instance;
        };

        virtual std::shared_ptr<PropertySetBase> createProperties()
        {
            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            PropertySetBase* properties = instance->getProperties();
            return std::shared_ptr<PropertySetBase>(properties);
        };
    };
}

#endif //SPACEINVADERS_BUILDER_HPP
