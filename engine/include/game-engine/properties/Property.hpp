//
// Created by adria on 02/02/2019.
//

#ifndef GAMEENGINE_PROPERTY_HPP
#define GAMEENGINE_PROPERTY_HPP

#include <game-engine/api.hpp>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <game-engine/FileType.hpp>
#include <game-engine/events/Subject.hpp>

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

    virtual void resetValueToDefault() = 0;
};

class PUBLICAPI PropertyObserver : public Observer<>
{
    std::function<void()> callback_;
    Subject<> *target_;
public:
    explicit PropertyObserver(Subject<> *property, std::function<void()> callback)
        : target_(property), callback_(std::move(callback))
    {
        assert(target_);
        assert(callback_);
        target_->registerObserver(this);
    }

    ~PropertyObserver() override
    {
        if(target_!= nullptr)
            target_->unregisterObserver(this);
    }
private:
    void onEvent(const Subject<> &target) override
    {
        assert(callback_);
        callback_();
    }
};

template<typename MemberType>
class PUBLICAPI Property : public PropertyBase, public Subject<>
{
    std::unique_ptr<MemberType> value_;
    MemberType default_;
    std::unique_ptr<PropertyObserver> observer_;
public:
    Property(const std::string &name, MemberType defaultValue) :
        Property(name, defaultValue, false)
    {
    };

    Property(const std::string &name, MemberType defaultValue, bool required) :
        PropertyBase(name, required)
    {
        default_ = defaultValue;
        type_ = PropertyTypeDeductive<MemberType>::type;
        resetValueToDefault();
    };

    virtual ~Property()
    {
        value_.reset();
    };

    virtual MemberType& get() const
    {
        return *value_.get();
    };

    MemberType defaultValue() const
    {
        return default_;
    };

    virtual void set(MemberType value)
    {
        value_ = std::make_unique<MemberType>(value);
        notify();
    };

    void copyFrom(const Property<MemberType> &original)
    {
        set(original.get());
    }

    virtual void getDefault(void *defaultValue) const
    {
        auto defaultValueConverted = static_cast<MemberType*>(defaultValue);
        *defaultValueConverted = default_;
    }

    virtual void resetValueToDefault()
    {
        set(default_);
    }

    void registerObserver(const std::function<void()> &observer)
    {
        observer_ = std::make_unique<PropertyObserver>(this, observer);
    }
};

class PUBLICAPI PropertyFilePath : public Property<std::string>
{
    FileType fileType_;
public:
    PropertyFilePath(const std::string &name, std::string defaultValue, const FileType &fileType) :
            Property<std::string>(name, defaultValue),
            fileType_(fileType)
    {
        type_ = PropertyTypes::FILEPATH;
    };

    PropertyFilePath(const std::string &name, std::string defaultValue, const FileType &fileType, bool required) :
            Property<std::string>(name, defaultValue, required),
            fileType_(fileType)
    {
        type_ = PropertyTypes::FILEPATH;
    };

    virtual ~PropertyFilePath() {};

    FileType getFileType() const {
        return fileType_;
    }
};

class PUBLICAPI PropertyEnum : public Property<std::string>
{
    std::vector<std::string> allowedValues_;
public:
    PropertyEnum(const std::string &name, std::string defaultValue, const std::vector<std::string> &allowedValues) :
            Property<std::string>(name, defaultValue),
            allowedValues_(allowedValues)
    {
        type_ = PropertyTypes::ENUM;
    };

    PropertyEnum(const std::string &name, std::string defaultValue, const std::vector<std::string> &allowedValues, bool required) :
            Property<std::string>(name, defaultValue, required),
            allowedValues_(allowedValues)
    {
        type_ = PropertyTypes::ENUM;
    };

    virtual ~PropertyEnum() {};

    std::vector<std::string> getAllowedValues() const {
        return allowedValues_;
    }

    void set(std::string value) override
    {
        auto it = std::find(allowedValues_.begin(), allowedValues_.end(), value);
        if(it == allowedValues_.end())
            throw std::invalid_argument("value " + value + " is not allowed in property " + name_);

        Property<std::string>::set(value);
    }
};
}

#endif //GAMEENGINE_PROPERTY_HPP
