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
    public:
        typedef MemberType Class::*Member;
        typedef MemberType (Class::*Getter)() const;
        typedef void (Class::*Setter)(const MemberType &);
    private:
        Member value_;
        Getter getter_;
        Setter setter_;
        MemberType default_;
        bool useMethods;
    public:
        Property(const std::string &name, Class *target, Member value, MemberType defaultValue) :
                Property(name, target, value, defaultValue, false){
        };

        Property(const std::string &name, Class *target, Member value, MemberType defaultValue, bool required) :
                PropertyBase(name, required),
                value_(value),
                getter_(nullptr),
                setter_(nullptr),
                useMethods(false) {
            default_ = defaultValue;
            target_ = target;
        };

        Property(const std::string &name, Class *target, Getter getter, Setter setter, MemberType defaultValue) :
                Property(name, target, getter, setter, defaultValue, false){
        };

        Property(const std::string &name, Class *target, Getter getter, Setter setter, MemberType defaultValue, bool required) :
                PropertyBase(name, required),
                value_(nullptr),
                getter_(getter),
                setter_(setter),
                useMethods(true) {
            default_ = defaultValue;
            target_ = target;
        };

        MemberType get() const {
            return useMethods? (target_->*getter_)() : target_->*value_;
        };

        MemberType defaultValue() const {
            return default_;
        };

        void set(MemberType value) {
            if(useMethods)
                (target_->*setter_)(value);
            else
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
