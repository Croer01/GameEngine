//
// Created by adria on 02/02/2019.
//

#ifndef SPACEINVADERS_PROPERTYSET_HPP
#define SPACEINVADERS_PROPERTYSET_HPP

#include <game-engine/api.hpp>
#include <game-engine/properties/Property.hpp>
#include <vector>

namespace GameEngine {

    class PUBLICAPI PropertySetBase {
    protected:
        void *target_;
    public:
        explicit PropertySetBase(void *target) : target_(target){};
        virtual ~PropertySetBase() = default;

        virtual void copy(PropertySetBase &other) const {
            throw std::runtime_error("not implemented. Need to cast this instance to PropertySet<Component>");
        };
    };

    template <typename Class>
    class PUBLICAPI PropertySet : public PropertySetBase {
        std::vector<PropertyBase<Class>*> properties_;
    public:
        explicit PropertySet(Class *target) : PropertySetBase(target)
        {};

        int size() const{
            return properties_.size();
        }

        void add(GameEngine::PropertyBase<Class> *property) {
            properties_.push_back(property);
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
            auto otherCast = static_cast<PropertySet<Class>&>(other);
            for (auto *property : properties_) {
                otherCast.add(property->copy(static_cast<Class*>(otherCast.target_)));
            }
        };
    };
}
#endif //SPACEINVADERS_PROPERTYSET_HPP
