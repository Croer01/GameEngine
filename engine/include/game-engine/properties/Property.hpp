//
// Created by adria on 02/02/2019.
//

#ifndef SPACEINVADERS_PROPERTY_HPP
#define SPACEINVADERS_PROPERTY_HPP

#include <game-engine/api.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <game-engine/FileType.h>

namespace GameEngine {

enum class PropertyTypes
{
    UNKNOWN,
    INT,
    FLOAT,
    STRING,
    BOOL,
    VEC2D,
    ARRAY_STRING,
    ARRAY_VEC2D,
    COLOR,
    FILEPATH,
    ENUM
};

template<typename ValueType>
struct PropertyTypeDeductive
{
    PropertyTypes type = PropertyTypes::UNKNOWN;
};
template<>
struct PropertyTypeDeductive<int>
{
    static constexpr PropertyTypes type = PropertyTypes::INT;
};
template<>
struct PropertyTypeDeductive<float>
{
    static constexpr PropertyTypes type = PropertyTypes::FLOAT;
};
template<>
struct PropertyTypeDeductive<std::string>
{
    static constexpr PropertyTypes type = PropertyTypes::STRING;
};
template<>
struct PropertyTypeDeductive<bool>
{
    static constexpr PropertyTypes type = PropertyTypes::BOOL;
};
template<>
struct PropertyTypeDeductive<GameEngine::Vec2D>
{
    static constexpr PropertyTypes type = PropertyTypes::VEC2D;
};
template<>
struct PropertyTypeDeductive<std::vector<std::string>>
{
    static constexpr PropertyTypes type = PropertyTypes::ARRAY_STRING;
};
template<>
struct PropertyTypeDeductive<std::vector<Vec2D>>
{
    static constexpr PropertyTypes type = PropertyTypes::ARRAY_VEC2D;
};
template<>
struct PropertyTypeDeductive<geColor>
{
    static constexpr PropertyTypes type = PropertyTypes::COLOR;
};

class PUBLICAPI PropertyBase
{
protected:
    PropertyTypes type_;
    std::string name_;
    bool required_;
public:
    explicit PropertyBase(const std::string &name) : PropertyBase(name, false)
    {};

    PropertyBase(const std::string &name, bool required) : name_(name), required_(required),
                                                           type_(PropertyTypes::UNKNOWN)
    {};

    virtual ~PropertyBase()
    {};

    PropertyTypes type() const
    {
        return type_;
    };

    std::string name() const
    {
        return name_;
    };

    bool required() const
    {
        return required_;
    };

    virtual void getDefault(void *defaultValue) const = 0;
};

template<typename Class>
class PUBLICAPI PropertyTBase : public PropertyBase
{
public:
    explicit PropertyTBase(const std::string &name) : PropertyBase(name, false)
    {};

    PropertyTBase(const std::string &name, bool required) : PropertyBase(name, required)
    {};

    virtual ~PropertyTBase()
    {};

    virtual void copy(const std::shared_ptr<const Class> &original, const std::shared_ptr<Class> &target) const = 0;

    virtual void getDefault(void *defaultValue) const = 0;

    virtual void resetValueToDefault(const std::shared_ptr<Class> &target) = 0;
};

template<typename Class, typename MemberType>
class PUBLICAPI Property : public PropertyTBase<Class>
{
public:
    typedef MemberType (Class::*Getter)() const;
    typedef void (Class::*Setter)(const MemberType &);
private:
    Getter getter_;
    Setter setter_;
    MemberType default_;
public:
    Property(const std::string &name, Getter getter, Setter setter, MemberType defaultValue) :
        Property(name, getter, setter, defaultValue, false)
    {
    };

    Property(const std::string &name, Getter getter, Setter setter, MemberType defaultValue, bool required) :
        PropertyTBase<Class>(name, required),
        getter_(getter),
        setter_(setter)
    {
        default_ = defaultValue;
        type_ = PropertyTypeDeductive<MemberType>::type;
    };

    virtual ~Property()
    {
        getter_ = nullptr;
        setter_ = nullptr;
    };

    virtual MemberType get(const Class *target) const
    {
        if (getter_ == nullptr)
            throw std::runtime_error("There is not a getter registered to use in property " + this->name_);

        return (target->*getter_)();
    };

    MemberType defaultValue() const
    {
        return default_;
    };

    virtual void set(Class *target, MemberType value) const
    {
        if (setter_ == nullptr)
            throw std::runtime_error("There is not a setter registered to use in property " + this->name_);
        (target->*setter_)(value);
    };

    virtual void copy(const std::shared_ptr<const Class> &original, const std::shared_ptr<Class> &target) const
    {
        set(target.get(), get(original.get()));
    }

    virtual void getDefault(void *defaultValue) const
    {
        MemberType *defaultValueConverted = static_cast<MemberType*>(defaultValue);
        *defaultValueConverted = default_;
    }

    virtual void resetValueToDefault(const std::shared_ptr<Class> &target)
    {
       set(target.get(), default_);
    }
};


class PUBLICAPI PropertyFilePathBase
{
public:
    virtual ~PropertyFilePathBase() {};
    virtual FileType getFileType() const = 0;
};


template<typename Class>
class PUBLICAPI PropertyFilePath : public Property<Class, std::string>, public PropertyFilePathBase
{
    FileType fileType_;
public:
    PropertyFilePath(const std::string &name, typename Property<Class, std::string>::Getter getter, typename Property<Class, std::string>::Setter setter, std::string defaultValue, const FileType &fileType) :
            Property(name, getter, setter, defaultValue),
            fileType_(fileType)
    {
        type_ = PropertyTypes::FILEPATH;
    };

    PropertyFilePath(const std::string &name, typename Property<Class, std::string>::Getter getter, typename Property<Class, std::string>::Setter setter, std::string defaultValue, const FileType &fileType, bool required) :
            Property(name, getter, setter, defaultValue, required),
            fileType_(fileType)
    {
        type_ = PropertyTypes::FILEPATH;
    };

    virtual ~PropertyFilePath() {};

    virtual FileType getFileType() const {
        return fileType_;
    }
};

class PUBLICAPI PropertyEnumBase
{
public:
    virtual ~PropertyEnumBase() {};
    virtual std::vector<std::string> getAllowedValues() const = 0;
};

template<typename Class>
class PUBLICAPI PropertyEnum : public Property<Class, std::string>, public PropertyEnumBase
{
    std::vector<std::string> allowedValues_;
public:
    PropertyEnum(const std::string &name, typename Property<Class, std::string>::Getter getter, typename Property<Class, std::string>::Setter setter, std::string defaultValue, const std::vector<std::string> &allowedValues) :
            Property(name, getter, setter, defaultValue),
            allowedValues_(allowedValues)
    {
        type_ = PropertyTypes::ENUM;
    };

    PropertyEnum(const std::string &name, typename Property<Class, std::string>::Getter getter, typename Property<Class, std::string>::Setter setter, std::string defaultValue, const std::vector<std::string> &allowedValues, bool required) :
            Property(name, getter, setter, defaultValue, required),
            allowedValues_(allowedValues)
    {
        type_ = PropertyTypes::ENUM;
    };

    virtual ~PropertyEnum() {};

    virtual std::vector<std::string> getAllowedValues() const {
        return allowedValues_;
    }

    void set(Class *target, std::string value) const override
    {
        auto it = std::find(allowedValues_.begin(), allowedValues_.end(), value);
        if(it == allowedValues_.end())
            throw std::invalid_argument("value " + value + " is not allowed in property " + name_);

        Property::set(target, value);
    }
};
}

#endif //SPACEINVADERS_PROPERTY_HPP
