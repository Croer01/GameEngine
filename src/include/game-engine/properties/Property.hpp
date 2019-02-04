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
        BOOL,
        VEC2D
    };

    template <typename Class>
    class PUBLICAPI PropertyBase{
    protected:
        Class *target_;
        // TODO: implement type
        PropertyTypes type_;
        std::string name_;
        bool required_;
    public:
        explicit PropertyBase(const std::string &name) : PropertyBase(name, false){};
        PropertyBase(const std::string &name, bool required) : name_(name), required_(required){};
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
        bool required() const {
            return required_;
        };
    };

    template <typename Class, typename MemberType>
    class PUBLICAPI Property : public PropertyBase<Class>{
        typedef MemberType Class::*Member;
        Member value_;
        MemberType default_;
    public:
        Property(const std::string &name, Class *target, Member value, MemberType defaultValue) :
                Property(name, target, value, defaultValue, false){
        };

        Property(const std::string &name, Class *target, Member value, MemberType defaultValue, bool required) :
                PropertyBase(name, required),
                value_(value) {
            default_ = defaultValue;
            target_ = target;
        };

        MemberType get() const {
            return target_->*value_;
        };

        MemberType defaultValue() const {
            return default_;
        };

        void set(MemberType value) {
            target_->*value_ = value;
        };

        PropertyBase<Class> copy(Class *newTarget) const override {
            Property<Class, MemberType> clone(name_, newTarget, value_, default_, required_);
            clone.target(newTarget);
            return clone;
        }
    };
}

#endif //SPACEINVADERS_PROPERTY_HPP
