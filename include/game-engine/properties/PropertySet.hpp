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
        void *target_;
    public:
        explicit PropertySetBase(void *target) : target_(target){};
        virtual ~PropertySetBase(){
            target_ = nullptr;
        };

        virtual void copy(PropertySetBase &other) const {
            throw std::runtime_error("not implemented. Need to cast this instance to PropertySet<Component>");
        };

        virtual void fillFrom(const geData &data) const {
            throw std::runtime_error("not implemented. Need to cast this instance to PropertySet<Component>");
        };
    };

    template <typename Class>
    class PUBLICAPI PropertySet : public PropertySetBase {
        std::vector<std::shared_ptr<PropertyBase<Class>>> properties_;
        std::unique_ptr<PropertySetBase> parent_;
    public:
        explicit PropertySet(Class *target) :
        PropertySetBase(target),
        parent_(nullptr)
        {};

        PropertySet(Class *target, PropertySetBase *parent) :
            PropertySetBase(target),
            parent_(parent)
        {};

        virtual ~PropertySet(){
            properties_.clear();
        }

        int size() const{
            return properties_.size();
        }

        void add(GameEngine::PropertyBase<Class> *property) {
            properties_.emplace_back(std::move(property));
        };

        PropertyBase<Class>& get(int index){
            return *properties_[index];
        }

        PropertyBase<Class>& get(const std::string &name) {
            int index = -1;
            int i = 0;

            while (index == -1 && i < properties_.size()){
                if(properties_[index]->name() == name)
                    index = i;
                i++;
            }

            if(index == -1)
                throw std::runtime_error("property " + name + " not found");

            return  *properties_[index];
        };

        virtual void copy(PropertySetBase &other) const {
            auto &otherCast = static_cast<PropertySet<Class>&>(other);

            if(parent_)
            {
                if(otherCast.parent_){
                    parent_->copy(*otherCast.parent_);
                } else{
                    throw new std::invalid_argument("other doesn't have the same parent structure");
                }
            }

            for( int i = 0; i < properties_.size(); i++) {
                otherCast.add(properties_[i]->copy(static_cast<Class*>(otherCast.target_)));
            }
        };

        virtual void fillFrom(const geData &data) const
        {
            if(parent_)
                parent_->fillFrom(data);

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
                        propertyInt->set(data.getInt(property->name()));
                    }
                        break;
                    case PropertyTypes::FLOAT:
                    {
                        auto propertyFloat = std::dynamic_pointer_cast<Property<Class,float>>(property);
                        propertyFloat->set(data.getFloat(property->name()));
                    }
                        break;
                    case PropertyTypes::STRING:
                    {
                        auto propertyString = std::dynamic_pointer_cast<Property<Class,std::string>>(property);
                        propertyString->set(data.getString(property->name()));
                    }
                        break;
                    case PropertyTypes::VEC2D:
                    {
                        auto propertyVec2d = std::dynamic_pointer_cast<Property<Class,Vec2D>>(property);
                        propertyVec2d->set(data.getVec2D(property->name()));
                    }
                        break;
                    case PropertyTypes::BOOL:
                    {
                        auto propertyBool = std::dynamic_pointer_cast<Property<Class, bool>>(property);
                        propertyBool->set(data.getBool(property->name()));
                    }
                        break;
                    case PropertyTypes::ARRAY_STRING:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<std::string>>>(property);
                        propertyArray->set(data.getArrayString(property->name()));
                    }
                        break;
                    case PropertyTypes::ARRAY_VEC2D:
                    {
                        auto propertyArray = std::dynamic_pointer_cast<Property<Class, std::vector<Vec2D>>>(property);
                        propertyArray->set(data.getArrayVec2D(property->name()));
                    }
                        break;
                    case PropertyTypes::COLOR:
                    {
                        auto propertyColor = std::dynamic_pointer_cast<Property<Class, geColor>>(property);
                        propertyColor->set(data.getColor(property->name()));
                    }
                        break;
                    case PropertyTypes::UNKNOWN:
                        throw std::runtime_error("the property " + property->name() + " has unknown type");
                        break;
                }
            }
        }
    };
}
#endif //SPACEINVADERS_PROPERTYSET_HPP
