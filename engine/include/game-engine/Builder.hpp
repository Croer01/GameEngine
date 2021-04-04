//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/geData.hpp>
#include <memory>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual geComponentRef Create() = 0;
        virtual geComponentRef Create(const geData &data) = 0;
        virtual ComponentDataRef createProperties() = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual geComponentRef Create() {
            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            ComponentDataRef compData = instance->instantiateData();
            instance->setData(compData);
            return instance;
        };

        virtual geComponentRef Create(const geData &data) {
            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            ComponentDataRef compData = instance->instantiateData();
            compData->fill(data);
            instance->setData(compData);
            return instance;
        };

        virtual ComponentDataRef createProperties()
        {
            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            return instance->instantiateData();
        };
    };
}

#endif //SPACEINVADERS_BUILDER_HPP
