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
        VEC2D,
        ARRAY_STRING
    };

    template <typename ValueType>
    struct PropertyTypeDeductive{
        PropertyTypes type = PropertyTypes::UNKNOWN;
    };
    template <>
    struct PropertyTypeDeductive<int>{
        static constexpr PropertyTypes type = PropertyTypes::INT;
    };
    template <>
    struct PropertyTypeDeductive<float>{
        static constexpr PropertyTypes type = PropertyTypes::FLOAT;
    };
    template <>
    struct PropertyTypeDeductive<std::string>{
        static constexpr PropertyTypes type = PropertyTypes::STRING;
    };
    template <>
    struct PropertyTypeDeductive<bool>{
        static constexpr PropertyTypes type = PropertyTypes::BOOL;
    };
    template <>
    struct PropertyTypeDeductive<GameEngine::Vec2D>{
        static constexpr PropertyTypes type = PropertyTypes::VEC2D;
    };
    template <>
    struct PropertyTypeDeductive<std::vector<std::string>>{
        static constexpr PropertyTypes type = PropertyTypes::ARRAY_STRING;
    };

    template <typename Class>
    class PUBLICAPI PropertyBase{
    protected:
        Class *target_;
        PropertyTypes type_;
        std::string name_;
        bool required_;
    public:
        explicit PropertyBase(const std::string &name) : PropertyBase(name, false){};
        PropertyBase(const std::string &name, bool required) : name_(name), required_(required), type_(PropertyTypes::UNKNOWN){};
        virtual PropertyBase<Class> *copy(Class *newTarget) const {
            throw std::runtime_error("not implemented. Cast to Property class to call this method");
        };
        void target(Class *target){
            target_ = target;
        };
        PropertyTypes type() const {
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
            if(value == nullptr)
                throw std::invalid_argument("the member must not be null.");
            default_ = defaultValue;
            target_ = target;
            type_ = PropertyTypeDeductive<MemberType>::type;
            set(default_);
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
            type_ = PropertyTypeDeductive<MemberType>::type;
            if(setter_ != nullptr)
                set(default_);
        };

        MemberType get() const {
            if(useMethods){
                if(getter_ == nullptr)
                    throw std::runtime_error("There is not a getter registered to use in property " + name_);

                return (target_->*getter_)();
            } else {
                return target_->*value_;
            }
        };

        MemberType defaultValue() const {
            return default_;
        };

        void set(MemberType value) {
            if(useMethods) {
                if(setter_ == nullptr)
                    throw std::runtime_error("There is not a setter registered to use in property " + name_);
                (target_->*setter_)(value);
            } else {
                target_->*value_ = value;
            }
        };

        virtual PropertyBase<Class> *copy(Class *newTarget) const override {
            Property<Class, MemberType> *clone = nullptr;
            if (useMethods)
                clone = new Property<Class, MemberType>(name_, newTarget, getter_, setter_, default_, required_);
            else
                clone = new Property<Class, MemberType>(name_, newTarget, value_, default_, required_);

            clone->set(get());
            return clone;
        }
    };
}

#endif //SPACEINVADERS_PROPERTY_HPP
