//
// Created by adria on 17/11/2019.
//

#ifndef GAMEENGINEEDITOR_PROPERTIESHOLDER_HPP
#define GAMEENGINEEDITOR_PROPERTIESHOLDER_HPP

#include <memory>
#include <game-engine/api.hpp>
#include <game-engine/properties/Property.hpp>
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/geData.hpp>

namespace GameEngine
{
class PUBLICAPI PropertiesHolder : public std::enable_shared_from_this<PropertiesHolder>
{
    std::unique_ptr<PropertiesBinderBase> properties_;

    template<typename T>
    void initializePropertiesBinding()
    {
        std::shared_ptr<PropertySetBase> properties = std::shared_ptr<PropertySetBase>(getProperties());
        if (!properties) {
            throw std::runtime_error(
                    std::string("there aren't properties defined. It can not possible initialize properties for ") +
                    typeid(T).name());
        }
        auto propertiesInstance = std::dynamic_pointer_cast<PropertySet<T>>(properties);
        const std::shared_ptr<T> &propertiesTarget = std::dynamic_pointer_cast<T>(shared_from_this());
        properties_.reset(new PropertiesBinder<T>(propertiesTarget, propertiesInstance));
    }

public:

    virtual ~PropertiesHolder()
    {}

    template<typename T>
    void copyProperties(const geData &data)
    {
        if (!properties_) {
            initializePropertiesBinding<T>();
        }
        properties_->fillFrom(data);
    }

    template<typename T>
    void copyProperties(const std::shared_ptr<const T> &other)
    {
        if (!properties_) {
            initializePropertiesBinding<T>();
        }

        auto properties = dynamic_cast<PropertiesBinder<T> *>(properties_.get());
        if (properties) {
            properties->fillFrom(other);
        }
    }

    virtual PropertySetBase *getProperties() const = 0;
};

}

#endif //GAMEENGINEEDITOR_PROPERTIESHOLDER_HPP
