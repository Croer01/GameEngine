//
// Created by adria on 02/02/2019.
//

#ifndef SPACEINVADERS_PROPERTY_HPP
#define SPACEINVADERS_PROPERTY_HPP

#include <string>
#include <game-engine/api.hpp>

namespace GameEngine {

    enum class PropertyTypes{
        UNKNOWN,
        INT,
        FLOAT,
        STRING,
        VEC2D
    };

    template <typename Class>
    class PUBLICAPI PropertyBase{
    protected:
        Class *target_;
        PropertyTypes type_;
        std::string name_;
    public:
        PropertyBase(const std::string &name) : name_(name){};
        virtual PropertyBase copy(Class *newTarget) const {
            throw std::runtime_error("not implemented. Cast to Property class to call this method");
        };
        void target(Class *target){
            target_ = target;
        }
        PropertyTypes type() const{
            return type_;
        };

        std::string name() const {
            return name_;
        };
    };

    template <typename Class, typename MemberType>
    class PUBLICAPI Property : public PropertyBase<Class>{
        typedef MemberType Class::*Member;
        Member value_;
    public:
        Property(const std::string &name, Class *target, Member value) :
            PropertyBase(name),
            value_(value) {
            target_ = target;
        };

        MemberType get() const {
            return target_->*value_;
        };

        void set(MemberType value) {
            target_->*value_ = value;
        };

        PropertyBase<Class> copy(Class *newTarget) const override {
            Property<Class, MemberType> clone(name_, newTarget, value_);
            clone.target(newTarget);
            return clone;
        }
    };
}

#endif //SPACEINVADERS_PROPERTY_HPP
