//
// Created by adria on 25/02/2021.
//

#ifndef GAMEENGINE_COMPONENTDATA_HPP
#define GAMEENGINE_COMPONENTDATA_HPP


#include <vector>
#include <memory>
#include <game-engine/geData.hpp>
#include <game-engine/properties/Property.hpp>

namespace GameEngine
{
class ComponentData;
typedef std::shared_ptr<ComponentData> ComponentDataRef;
class ComponentData
{
    std::vector<std::shared_ptr<PropertyBase>> properties_;

    std::shared_ptr<PropertyBase> findExists(const std::string &name)
    {
        std::shared_ptr<PropertyBase> result;

        if (!result)
        {
            auto it = std::find_if(properties_.begin(), properties_.end(), [&name](const std::shared_ptr<PropertyBase> &p)
            {
                return p->name() == name;
            });
            if (it != properties_.end())
                result = *it;
        }

        return result;
    }

protected:

    template<typename DataType>
    Property<DataType> *createProperty(const std::string &name, DataType defaultValue, bool required = false)
    {
        if (findExists(name))
            throw std::runtime_error("The property " + name + " already exists");

        auto prop = std::make_shared<Property<DataType>>(name, defaultValue, required);
        properties_.push_back(prop);
        return prop.get();
    }

    PropertyEnum *createPropertyEnum(const std::string &name, const std::string &defaultValue,
                                     const std::vector<std::string> &allowedValues, bool required = false)
    {
        if (findExists(name))
            throw std::runtime_error("The property " + name + " already exists");

        auto prop = std::make_shared<PropertyEnum>(name, defaultValue, allowedValues, required);
        properties_.push_back(prop);
        return prop.get();
    }

    PropertyFilePath *createPropertyFilePath(const std::string &name, const std::string &defaultValue, const FileType &fileType, bool required = false)
    {
        if (findExists(name))
            throw std::runtime_error("The property " + name + " already exists");

        auto prop = std::make_shared<PropertyFilePath>(name, defaultValue, fileType, required);
        properties_.push_back(prop);
        return prop.get();
    }

public:
    virtual ~ComponentData()
    {}

    // Iterator Begin
    typedef typename std::vector<std::shared_ptr<PropertyBase>>::iterator iterator;
    typedef typename std::vector<std::shared_ptr<PropertyBase>>::const_iterator const_iterator;

    inline iterator begin() noexcept { return properties_.begin(); }
    inline const_iterator cbegin() const noexcept { return properties_.cbegin(); }
    inline iterator end() noexcept { return properties_.end(); }
    inline const_iterator cend() const noexcept { return properties_.cend(); }
    // Iterator End

    template<typename DataType>
    Property<DataType> *getProperty(const std::string &name)
    {
        auto prop = findExists(name);

        if (!prop)
            throw std::runtime_error("The property " + name + " doesn't exist");

        if (prop->type() != PropertyTypeDeductive<DataType>::type)
        {
            if(prop->type() == PropertyTypes::FILEPATH)
                throw std::runtime_error("The property " + name + " is a file type property. Use getFilePathProperty instead of getProperty<T>");
            else if(prop->type() == PropertyTypes::ENUM)
                throw std::runtime_error("The property " + name + " is a file type property. Use getEnumProperty instead of getProperty<T>");
            else
                throw std::runtime_error("The property " + name + " has a different type that the expected from get");
        }

        return std::dynamic_pointer_cast<Property<DataType>>(prop).get();
    }

    PropertyFilePath *getFilePathProperty(const std::string &name)
    {
        auto prop = findExists(name);

        if (!prop)
            throw std::runtime_error("The property " + name + " doesn't exist");

        if(prop->type() == PropertyTypes::ENUM)
            throw std::runtime_error("The property " + name + " is a file type property. Use getEnumProperty instead of getFilePathProperty");
        else if (prop->type() != PropertyTypes::FILEPATH)
            throw std::runtime_error("The property " + name + " is not a file type property. Use getProperty<T> instead of getFilePathProperty");

        auto ptr = std::dynamic_pointer_cast<PropertyFilePath>(prop);
        return ptr.get();
    }

    PropertyEnum *getEnumProperty(const std::string &name)
    {
        auto prop = findExists(name);

        if (!prop)
            throw std::runtime_error("The property " + name + " doesn't exist");

        if(prop->type() == PropertyTypes::FILEPATH)
            throw std::runtime_error("The property " + name + " is a file type property. Use getFilePathProperty instead of getEnumProperty");
        else if (prop->type() != PropertyTypes::ENUM)
            throw std::runtime_error("The property " + name + " is not a file type property. Use getProperty<T> instead of getEnumProperty");

        return std::dynamic_pointer_cast<PropertyEnum>(prop).get();
    }

    void fill(const geData &data)
    {
        for(int i = 0; i < properties_.size(); i++)
        {
            auto property = properties_[i];

            if(!data.hasValue(property->name())) {
                if(property->required())
                    throw std::logic_error("property " + property->name() + " is required but it isn't defined");
                continue;
            }

            switch (property->type()){
                case PropertyTypes::INT:
                {
                    auto propertyInt = std::dynamic_pointer_cast<Property<int>>(property);
                    propertyInt->set(data.getInt(property->name()));
                }
                    break;
                case PropertyTypes::FLOAT:
                {
                    auto propertyFloat = std::dynamic_pointer_cast<Property<float>>(property);
                    propertyFloat->set(data.getFloat(property->name()));
                }
                    break;
                case PropertyTypes::STRING:
                {
                    auto propertyString = std::dynamic_pointer_cast<Property<std::string>>(property);
                    propertyString->set(data.getString(property->name()));
                }
                    break;
                case PropertyTypes::VEC2D:
                {
                    auto propertyVec2d = std::dynamic_pointer_cast<Property<Vec2D>>(property);
                    propertyVec2d->set(data.getVec2D(property->name()));
                }
                    break;
                case PropertyTypes::BOOL:
                {
                    auto propertyBool = std::dynamic_pointer_cast<Property<bool>>(property);
                    propertyBool->set(data.getBool(property->name()));
                }
                    break;
                case PropertyTypes::ARRAY_STRING:
                {
                    auto propertyArray = std::dynamic_pointer_cast<Property<std::vector<std::string>>>(property);
                    propertyArray->set(data.getArrayString(property->name()));
                }
                    break;
                case PropertyTypes::ARRAY_VEC2D:
                {
                    auto propertyArray = std::dynamic_pointer_cast<Property<std::vector<Vec2D>>>(property);
                    propertyArray->set(data.getArrayVec2D(property->name()));
                }
                    break;
                case PropertyTypes::COLOR:
                {
                    auto propertyColor = std::dynamic_pointer_cast<Property<geColor>>(property);
                    propertyColor->set(data.getColor(property->name()));
                }
                    break;
                case PropertyTypes::FILEPATH:
                {
                    auto propertyFilepath = std::dynamic_pointer_cast<PropertyFilePath>(property);
                    propertyFilepath->set(data.getString(property->name()));
                }
                    break;
                case PropertyTypes::ENUM:
                {
                    auto propertyEnum = std::dynamic_pointer_cast<PropertyEnum>(property);
                    propertyEnum->set(data.getString(property->name()));
                }
                    break;
                case PropertyTypes::UNKNOWN:
                    throw std::runtime_error("the property " + property->name() + " has unknown type");
                    break;
            }
        }
    }

    template<class CompDataT>
    std::shared_ptr<ComponentData> clone()
    {
        auto other = std::make_shared<CompDataT>();

        for (auto property : properties_)
        {
            switch (property->type())
            {
                case PropertyTypes::INT:
                {
                    auto propertyInt = std::dynamic_pointer_cast<Property<int>>(property);
                    other->template getProperty<int>(property->name())->copyFrom(*propertyInt);
                }
                    break;
                case PropertyTypes::FLOAT:
                {
                    auto propertyFloat = std::dynamic_pointer_cast<Property<float>>(property);
                    other->template getProperty<float>(property->name())->copyFrom(*propertyFloat);
                }
                    break;
                case PropertyTypes::STRING:
                {
                    auto propertyString = std::dynamic_pointer_cast<Property<std::string>>(property);
                    other->template getProperty<std::string>(property->name())->copyFrom(*propertyString);
                }
                    break;
                case PropertyTypes::VEC2D:
                {
                    auto propertyVec2d = std::dynamic_pointer_cast<Property<Vec2D>>(property);
                    other->template getProperty<Vec2D>(property->name())->copyFrom(*propertyVec2d);
                }
                    break;
                case PropertyTypes::BOOL:
                {
                    auto propertyBool = std::dynamic_pointer_cast<Property<bool>>(property);
                    other->template getProperty<bool>(property->name())->copyFrom(*propertyBool);
                }
                    break;
                case PropertyTypes::ARRAY_STRING:
                {
                    auto propertyArray = std::dynamic_pointer_cast<Property<std::vector<std::string>>>(property);
                    other->template getProperty<std::vector<std::string>>(property->name())->copyFrom(*propertyArray);
                }
                    break;
                case PropertyTypes::ARRAY_VEC2D:
                {
                    auto propertyArray = std::dynamic_pointer_cast<Property<std::vector<Vec2D>>>(property);
                    other->template getProperty<std::vector<Vec2D>>(property->name())->copyFrom(*propertyArray);
                }
                    break;
                case PropertyTypes::COLOR:
                {
                    auto propertyColor = std::dynamic_pointer_cast<Property<geColor>>(property);
                    other->template getProperty<geColor>(property->name())->copyFrom(*propertyColor);
                }
                    break;
                case PropertyTypes::FILEPATH:
                {
                    auto propertyFilepath = std::dynamic_pointer_cast<PropertyFilePath>(property);
                    other->getFilePathProperty(property->name())->copyFrom(*propertyFilepath);
                }
                    break;
                case PropertyTypes::ENUM:
                {
                    auto propertyEnum = std::dynamic_pointer_cast<PropertyEnum>(property);
                    other->getEnumProperty(property->name())->copyFrom(*propertyEnum);
                }
                    break;
                case PropertyTypes::UNKNOWN:
                    throw std::runtime_error("the property " + property->name() + " has unknown type");
                    break;
            }
        }

        return other;
    };
};
}


#endif //GAMEENGINE_COMPONENTDATA_HPP
