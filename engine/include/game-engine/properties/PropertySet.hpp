//
// Created by adria on 02/02/2019.
//

#ifndef SPACEINVADERS_PROPERTYSET_HPP
#define SPACEINVADERS_PROPERTYSET_HPP

#include <game-engine/api.hpp>
#include <game-engine/properties/Property.hpp>
#include <vector>
#include <memory>
#include <game-engine/geData.hpp>

namespace GameEngine {

    class PUBLICAPI PropertySetBase {
    protected:
        std::vector<std::shared_ptr<PropertyBase>> properties_;
        std::unique_ptr<PropertySetBase> parent_;
    public:
        PropertySetBase() :
            parent_(nullptr)
        {};

        explicit PropertySetBase(PropertySetBase *parent) :
        parent_(parent)
            {};

        virtual ~PropertySetBase(){
            properties_.clear();
        }

        PropertyBase& get(int index){
            if(index >= properties_.size() && parent_)
                return parent_->get(index - properties_.size());
            return *properties_[index];
        }

        PropertyBase& get(const std::string &name) {
            int index = -1;
            int i = 0;

            while (index == -1 && i < properties_.size()){
                if(properties_[i]->name() == name)
                    index = i;
                i++;
            }

            if(index == -1)
            {
                if(parent_)
                    return parent_->get(name);

                throw std::runtime_error("property " + name + " not found");
            }

            return  *properties_[index];
        }

        int size() const{
            int totalSize = properties_.size();
            if(parent_)
                totalSize += parent_->size();
            return totalSize;
        }

        virtual void resetValuesToDefault(const std::shared_ptr<void> &target) const = 0;

        virtual void copy(const std::shared_ptr<const void> &original, const std::shared_ptr<void> &target) const = 0;
        virtual void copy(const geData &data, const std::shared_ptr<void> &target) const = 0;
    };

    template <typename Class>
    class PUBLICAPI PropertySet : public PropertySetBase {
    public:
        PropertySet() : PropertySetBase()
        {};

        explicit PropertySet(PropertySetBase *parent) :
            PropertySetBase(parent)
        {};

        virtual ~PropertySet(){
        }

        void add(PropertyBase *property) {
            properties_.emplace_back(property);
        };

        virtual void resetValuesToDefault(const std::shared_ptr<void> &target) const
        {
            const std::shared_ptr<Class> &sharedPtr = std::static_pointer_cast<Class>(target);

            if(parent_)
                parent_->resetValuesToDefault(target);

            for( auto property : properties_) {
                property->resetValueToDefault(sharedPtr);
            }
        }

        virtual void copy(const std::shared_ptr<const void> &original, const std::shared_ptr<void> &target) const
        {
            const std::shared_ptr<const Class> &ptr = std::static_pointer_cast<const Class>(original);
            const std::shared_ptr<Class> &sharedPtr = std::static_pointer_cast<Class>(target);
            copyByType(ptr, sharedPtr);
        };
        virtual void copy(const geData &data, const std::shared_ptr<void> &target) const {

            const std::shared_ptr<Class> &sharedPtr = std::static_pointer_cast<Class>(target);
            copyByType(data, sharedPtr);
        };

        void copyByType(const std::shared_ptr<const Class> &original, const std::shared_ptr<Class> &target) const {
            if(parent_)
                parent_->copy(original, target);

            for( auto property : properties_)
            {
                switch (property->type()){
                    case PropertyTypes::INT:
                    {
                        auto propertyInt = std::dynamic_pointer_cast<Property<Class,int>>(property);
                        propertyInt->copy(original, target);
                    }
                        break;
                    case PropertyTypes::FLOAT:
                    {
                        auto propertyFloat = std::dynamic_pointer_cast<Property<Class,float>>(property);
                        propertyFloat->copy(original, target);
                    }
                        break;
                    case PropertyTypes::STRING:
                    {
                        auto propertyString = std::dynamic_pointer_cast<Property<Class,std::string>>(property);
                        propertyString->copy(original, target);
                    }
                        break;
                    case PropertyTypes::VEC2D:
                    {
                        auto propertyVec2d = std::dynamic_pointer_cast<Property<Class,Vec2D>>(property);
                        propertyVec2d->copy(original, target);
                    }
                        break;
                    case PropertyTypes::BOOL:
                    {
                        auto propertyBool = std::dynamic_pointer_cast<Property<Class, bool>>(property);
                        propertyBool->copy(original, target);
                    }
                        break;
                    case PropertyTypes::ARRAY_STRING:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<std::string>>>(property);
                        propertyArray->copy(original, target);
                    }
                        break;
                    case PropertyTypes::ARRAY_VEC2D:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<Vec2D>>>(property);
                        propertyArray->copy(original, target);
                    }
                        break;
                    case PropertyTypes::COLOR:
                    {
                        auto propertyColor = std::dynamic_pointer_cast<Property<Class, geColor>>(property);
                        propertyColor->copy(original, target);
                    }
                        break;
                    case PropertyTypes::FILEPATH:
                    {
                        auto propertyFilepath = std::dynamic_pointer_cast<PropertyFilePath<Class>>(property);
                        propertyFilepath->copy(original, target);
                    }
                        break;
                    case PropertyTypes::ENUM:
                    {
                        auto propertyEnum = std::dynamic_pointer_cast<PropertyEnum<Class>>(property);
                        propertyEnum->copy(original, target);
                    }
                        break;
                    case PropertyTypes::UNKNOWN:
                        throw std::runtime_error("the property " + property->name() + " has unknown type");
                        break;
                }
            }
        };

        void copyByType(const geData &data, const std::shared_ptr<Class> &target) const
        {
            if(parent_)
                parent_->copy(data, target);

            for(int i = 0; i < properties_.size(); i++)
            {
                auto property = properties_[i];

                if(!data.hasValue(property->name())) {
                    if(property->required())
                        throw std::logic_error("property " + property->name() + " is required but it isn't defined");
                    else
                    {
                        property->resetValueToDefault(target);
                    }
                    continue;
                }

                switch (property->type()){
                    case PropertyTypes::INT:
                    {
                        auto propertyInt = std::dynamic_pointer_cast<Property<Class,int>>(property);
                        propertyInt->set(target.get(), data.getInt(property->name()));
                    }
                        break;
                    case PropertyTypes::FLOAT:
                    {
                        auto propertyFloat = std::dynamic_pointer_cast<Property<Class,float>>(property);
                        propertyFloat->set(target.get(), data.getFloat(property->name()));
                    }
                        break;
                    case PropertyTypes::STRING:
                    {
                        auto propertyString = std::dynamic_pointer_cast<Property<Class,std::string>>(property);
                        propertyString->set(target.get(), data.getString(property->name()));
                    }
                        break;
                    case PropertyTypes::VEC2D:
                    {
                        auto propertyVec2d = std::dynamic_pointer_cast<Property<Class,Vec2D>>(property);
                        propertyVec2d->set(target.get(), data.getVec2D(property->name()));
                    }
                        break;
                    case PropertyTypes::BOOL:
                    {
                        auto propertyBool = std::dynamic_pointer_cast<Property<Class, bool>>(property);
                        propertyBool->set(target.get(), data.getBool(property->name()));
                    }
                        break;
                    case PropertyTypes::ARRAY_STRING:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<std::string>>>(property);
                        propertyArray->set(target.get(), data.getArrayString(property->name()));
                    }
                        break;
                    case PropertyTypes::ARRAY_VEC2D:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<Vec2D>>>(property);
                        propertyArray->set(target.get(), data.getArrayVec2D(property->name()));
                    }
                        break;
                    case PropertyTypes::COLOR:
                    {
                        auto propertyColor = std::dynamic_pointer_cast<Property<Class, geColor>>(property);
                        propertyColor->set(target.get(), data.getColor(property->name()));
                    }
                        break;
                    case PropertyTypes::FILEPATH:
                    {
                        auto propertyFilepath = std::dynamic_pointer_cast<PropertyFilePath<Class>>(property);
                        propertyFilepath->set(target.get(), data.getString(property->name()));
                    }
                        break;
                    case PropertyTypes::ENUM:
                    {
                        auto propertyEnum = std::dynamic_pointer_cast<PropertyEnum<Class>>(property);
                        propertyEnum->set(target.get(), data.getString(property->name()));
                    }
                        break;
                    case PropertyTypes::UNKNOWN:
                        throw std::runtime_error("the property " + property->name() + " has unknown type");
                        break;
                }
            }
        }
    };

class PUBLICAPI PropertiesBinderBase
{
public:
    virtual ~PropertiesBinderBase() {};

    virtual void fillFrom(const geData &data) const = 0;
};
template<typename Class>
class PUBLICAPI PropertiesBinder : public PropertiesBinderBase
{
    // target_ must be weak to avoid memory leaks
    std::weak_ptr<Class> target_;
    std::shared_ptr<PropertySet<Class>> properties_;
public:
    PropertiesBinder(const std::weak_ptr<Class> target, const std::shared_ptr<PropertySet<Class>> &properties) :
    target_(target), properties_(properties)
    {};

    virtual void fillFrom(const geData &data) const
    {
        properties_->copy(data, target_.lock());
    }

    void fillFrom(const std::shared_ptr<const Class> &other) const
    {
        properties_->copy(other, target_.lock());
    };

};

}
#endif //SPACEINVADERS_PROPERTYSET_HPP
