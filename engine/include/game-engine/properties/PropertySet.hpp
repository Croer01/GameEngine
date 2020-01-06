//
// Created by adria on 02/02/2019.
//

#ifndef SPACEINVADERS_PROPERTYSET_HPP
#define SPACEINVADERS_PROPERTYSET_HPP

#include <game-engine/api.hpp>
#include <game-engine/properties/Property.hpp>
#include <vector>
#include <memory>

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

        void add(PropertyTBase<Class> *property) {
            properties_.emplace_back(dynamic_cast<PropertyBase *>(std::move(property)));
        };

        void copy(const std::shared_ptr<const Class> &original, const std::shared_ptr<Class> &target) const {
            if(parent_)
            {
                auto parent = dynamic_cast<PropertySet<Class> *>(parent_.get());
                parent->copy(original, target);
            }

            for( int i = 0; i < properties_.size(); i++) {
                const std::shared_ptr<PropertyTBase<Class>> property = std::dynamic_pointer_cast<PropertyTBase<Class>>(properties_[i]);
                property->copy(original, target);
            }
        };

        void copy(const geData &data, const std::shared_ptr<Class> &target) const
        {
            if(parent_)
            {
                auto parent = dynamic_cast<PropertySet<Class> *>(parent_.get());
                parent->copy(data, target);
            }

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
    std::weak_ptr<Class> target_;
    std::weak_ptr<PropertySet<Class>> properties_;
public:
    PropertiesBinder(const std::weak_ptr<Class> target, const std::weak_ptr<PropertySet<Class>> &properties) :
    target_(target), properties_(properties)
    {};

    virtual void fillFrom(const geData &data) const
    {
        properties_.lock()->copy(data, target_.lock());
    }

    void fillFrom(const std::shared_ptr<const Class> &other) const
    {
        properties_.lock()->copy(other, target_.lock());
    };

};

}
#endif //SPACEINVADERS_PROPERTYSET_HPP
