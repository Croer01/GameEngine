//
// Created by adria on 11/10/2019.
//

#ifndef GAMEENGINEEDITOR_VIEWMODELS_HPP
#define GAMEENGINEEDITOR_VIEWMODELS_HPP


#include <string>
#include <vector>
#include <memory>
#include <array>
#include <yaml-cpp/yaml.h>
#include <game-engine/api.hpp>
#include <boost/filesystem/path.hpp>
#include <game-engine/events/Subject.hpp>
#include "GameComponentsProvider.hpp"
#include "DataFile.h"

struct Vector2DData
{
    std::array<float, 2> xy;
    Vector2DData();
};

struct ColorData
{
    std::array<float, 3> rgb;
    // color white as default
    ColorData();
};

enum class PropertyDataType
{
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

class PropertyData;
typedef std::shared_ptr<PropertyData> PropertyDataRef;
class PropertyData
{
public:
    PropertyDataType type_;
    std::string name_;
    bool requrired_;

    explicit PropertyData(const std::string &name) :
        name_(name), requrired_(false)
    {};
    virtual ~PropertyData(){};
};

class PropertyIntData : public PropertyData
{
public:
    int value_;
    explicit PropertyIntData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::INT;
        value_ = 0;
    };
};

class PropertyFloatData : public PropertyData
{
public:
    float value_;
    explicit PropertyFloatData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::FLOAT;
        value_ = .0f;
    };
};

class PropertyStringData : public PropertyData
{
public:
    std::string value_;
    explicit PropertyStringData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::STRING;
    };
};

class PropertyBoolData : public PropertyData
{
public:
    bool value_;
    explicit PropertyBoolData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::BOOL;
        value_ = false;
    };
};

class PropertyVec2DData : public PropertyData
{
public:
    Vector2DData value_;
    explicit PropertyVec2DData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::VEC2D;
    };
};

class PropertyStringArrayData : public PropertyData
{
public:
    std::vector<std::string> value_;
    explicit PropertyStringArrayData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::ARRAY_STRING;
    };
};

class PropertyVec2DArrayData : public PropertyData
{
public:
    std::vector<Vector2DData> value_;
    explicit PropertyVec2DArrayData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::ARRAY_VEC2D;
    };
};

class PropertyColorData : public PropertyData
{
public:
    ColorData value_;
    explicit PropertyColorData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::COLOR;
    };
};

class PropertyFilePathData : public PropertyData
{
public:
    DataFileType fileTypeSupported_;
    DataFile value_;
    explicit PropertyFilePathData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::FILEPATH;
        fileTypeSupported_ = DataFileType ::Other;
    };
};

class PropertyEnumData : public PropertyData
{
public:
    std::string value_;
    std::vector<std::string> allowedValues_;

    explicit PropertyEnumData(const std::string &name) : PropertyData(name)
    {
        type_ = PropertyDataType::ENUM;
    };
};

class ComponentData;
typedef std::shared_ptr<ComponentData> ComponentDataRef;
class ComponentData
{
public:
    std::string name_;
    std::vector<PropertyDataRef> properties_;
};

class ObjectData;
typedef std::shared_ptr<ObjectData> ObjectDataRef;

class ObjectData
{
public:
    ObjectData();
    Vector2DData position_;
    Vector2DData scale_;
    float rotation_;
    std::string name_;
    std::vector<ObjectDataRef> children_;
    std::vector<ComponentDataRef> components_;
};

enum class PrototypeReferenceEvent
{
    PositionChanged,
    RotationChanged,
    ScaleChanged
};

class PrototypeReference;
typedef std::shared_ptr<PrototypeReference> PrototypeReferenceRef;
class PrototypeReference : public GameEngine::Subject<PrototypeReferenceEvent>
{
    Vector2DData position_;
    Vector2DData scale_;
    float rotation_;
public:
    std::string name_;
    std::string prototype_;
    PrototypeReference();
    Vector2DData getPosition() const;
    void setPosition(const Vector2DData &position);
    Vector2DData getScale() const;
    void setScale(const Vector2DData &scale);
    float getRotation() const;
    void setRotation(float rotation);
};

enum class SceneDataEvent
{
    ObjectAdded,
    ObjectDeleted,
    ObjectSelected
};

class SceneData;
typedef std::shared_ptr<SceneData> SceneDataRef;
class SceneData : public GameEngine::Subject<SceneDataEvent>
{
    std::vector<PrototypeReferenceRef> objects_;
    PrototypeReferenceRef selected_;

public:
    typedef std::vector<PrototypeReferenceRef>::const_iterator const_iterator;

    virtual ~SceneData(){}
    std::string name_;
    std::string filePath_;
    void addObject(const PrototypeReferenceRef &object);
    size_t objectsSize() const;
    PrototypeReferenceRef getObject(int index) const;
    void deleteObject(int index);
    void selectObject(const PrototypeReferenceRef &object);
    PrototypeReferenceRef getSelectedObject() const;
    const_iterator getObjectsBegin() const;
    const_iterator getObjectsEnd() const;

};

class PhysicsCategory;
typedef std::shared_ptr<PhysicsCategory> PhysicsCategoryRef;
class PhysicsCategory
{
public:
    std::string name_;
    std::vector<std::string> mask_;
};

class ProjectData;
typedef std::shared_ptr<ProjectData> ProjectDataRef;
class ProjectData
{
public:
    std::string folderPath_;
    std::string folderName_;
    std::string currentScenePath_;
    // empty string means that the path doesn't exist
    boost::filesystem::path dataPath_;
    std::vector<PhysicsCategory> physicsCategories_;
    bool dirty_ = false;
};

namespace YAML {
template<>
struct convert<Vector2DData> {
    static Node encode(const Vector2DData &rhs) {
        Node node;
        node.push_back(rhs.xy[0]);
        node.push_back(rhs.xy[1]);
        return node;
    }

    static bool decode(const Node &node, Vector2DData &rhs) {
        if (!node.IsSequence() || node.size() != 2) {
            return false;
        }

        rhs.xy = node.as<std::array<float,2>>();
        return true;
    }
};

template<>
struct convert<ColorData> {
    static Node encode(const ColorData &rhs) {
        Node node;
        node.push_back(rhs.rgb[0]);
        node.push_back(rhs.rgb[1]);
        node.push_back(rhs.rgb[2]);
        return node;
    }

    static bool decode(const Node &node, ColorData &rhs) {
        if (!node.IsSequence() || node.size() != 3) {
            return false;
        }

        rhs.rgb = node.as<std::array<float,3>>();
        return true;
    }
};

template<>
struct convert<ComponentData> {
    static Node encode(const ComponentData &rhs) {
        Node node;
        node["type"] = rhs.name_;

        for(auto it = rhs.properties_.begin(); it != rhs.properties_.end(); ++it)
        {
            Node propertyNode;
            auto property = *it;

            switch (property->type_)
            {
                case PropertyDataType::INT: {
                    auto propertyInt = std::dynamic_pointer_cast<PropertyIntData>(property);
                    propertyNode = propertyInt->value_;
                }
                    break;
                case PropertyDataType::FLOAT: {
                    auto propertyFloat = std::dynamic_pointer_cast<PropertyFloatData>(property);
                    propertyNode = propertyFloat->value_;
                }
                    break;
                case PropertyDataType::STRING: {
                    auto propertyStringData = std::dynamic_pointer_cast<PropertyStringData>(property);
                    propertyNode = propertyStringData->value_;
                }
                    break;
                case PropertyDataType::BOOL: {
                    auto propertyBoolData = std::dynamic_pointer_cast<PropertyBoolData>(property);
                    propertyNode = propertyBoolData->value_;
                }
                    break;
                case PropertyDataType::VEC2D: {
                    auto propertyVec2DData = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    propertyNode = propertyVec2DData->value_;
                }
                    break;
                case PropertyDataType::ARRAY_STRING: {
                    auto propertyStringArrayData = std::dynamic_pointer_cast<PropertyStringArrayData>(property);
                    propertyNode = propertyStringArrayData->value_;
                }
                    break;
                case PropertyDataType::ARRAY_VEC2D: {
                    auto propertyVec2DArrayData = std::dynamic_pointer_cast<PropertyVec2DArrayData>(property);
                    propertyNode = propertyVec2DArrayData->value_;
                }
                    break;
                case PropertyDataType::COLOR: {
                    auto propertyColorData = std::dynamic_pointer_cast<PropertyColorData>(property);
                    propertyNode = propertyColorData->value_;
                }
                    break;
                case PropertyDataType::FILEPATH: {
                    auto propertyFilePathData = std::dynamic_pointer_cast<PropertyFilePathData>(property);
                    propertyNode = propertyFilePathData->value_.getFilePath().string();
                }
                    break;
                case PropertyDataType::ENUM: {
                    auto propertyEnumData = std::dynamic_pointer_cast<PropertyEnumData>(property);
                    propertyNode = propertyEnumData->value_;
                }
                    break;
                default:
                    assert(false && "unknown property type");
                    break;
            }

            node[property->name_] = propertyNode;
        }

        return node;
    }

    static bool decode(const Node &node, ComponentData &rhs) {
        GameComponentsProvider provider;
        rhs.name_ = node["type"].as<std::string>();
        rhs.properties_ = provider.getPropertiesMetadataByComponent(rhs.name_);

        for(auto it = rhs.properties_.begin(); it != rhs.properties_.end(); ++it)
        {
            auto property = *it;

            if(!node[property->name_].IsDefined())
            {
                if(property->requrired_)
                {
                    return false;
                }
                // if is not required, the property will be ignored
                assert(!property->requrired_);
                continue;
            }

            switch (property->type_)
            {
                case PropertyDataType::INT: {
                    auto propertyInt = std::dynamic_pointer_cast<PropertyIntData>(property);
                    propertyInt->value_ = node[property->name_].as<int>();
                }
                    break;
                case PropertyDataType::FLOAT: {
                    auto propertyFloat = std::dynamic_pointer_cast<PropertyFloatData>(property);
                    propertyFloat->value_ = node[property->name_].as<float>();
                }
                    break;
                case PropertyDataType::STRING: {
                    auto propertyStringData = std::dynamic_pointer_cast<PropertyStringData>(property);
                    propertyStringData->value_ = node[property->name_].as<std::string>();
                }
                    break;
                case PropertyDataType::BOOL: {
                    auto propertyBoolData = std::dynamic_pointer_cast<PropertyBoolData>(property);
                    propertyBoolData->value_ = node[property->name_].as<bool>();
                }
                    break;
                case PropertyDataType::VEC2D: {
                    auto propertyVec2DData = std::dynamic_pointer_cast<PropertyVec2DData>(property);
                    propertyVec2DData->value_ = node[property->name_].as<Vector2DData>();
                }
                    break;
                case PropertyDataType::ARRAY_STRING: {
                    auto propertyStringArrayData = std::dynamic_pointer_cast<PropertyStringArrayData>(property);
                    propertyStringArrayData->value_ = node[property->name_].as<std::vector<std::string>>();
                }
                    break;
                case PropertyDataType::ARRAY_VEC2D: {
                    auto propertyVec2DArrayData = std::dynamic_pointer_cast<PropertyVec2DArrayData>(property);
                    propertyVec2DArrayData->value_ = node[property->name_].as<std::vector<Vector2DData>>();
                }
                    break;
                case PropertyDataType::COLOR: {
                    auto propertyColorData = std::dynamic_pointer_cast<PropertyColorData>(property);
                    propertyColorData->value_ = node[property->name_].as<ColorData>();
                }
                    break;
                case PropertyDataType::FILEPATH: {
                    auto propertyFilePathData = std::dynamic_pointer_cast<PropertyFilePathData>(property);
                    propertyFilePathData->value_ = DataFile(node[property->name_].as<std::string>());
                }
                    break;
                case PropertyDataType::ENUM: {
                    auto propertyEnumData = std::dynamic_pointer_cast<PropertyEnumData>(property);
                    propertyEnumData->value_ = node[property->name_].as<std::string>();
                }
                    break;
                default:
                    return false;
            }
        }

        return true;
    }
};

template<>
struct convert<ObjectData> {
    static Node encode(const ObjectData &rhs) {
        Node node;
        node["name"] = rhs.name_;
        node["position"] = rhs.position_;
        node["rotation"] = rhs.rotation_;
        node["scale"] = rhs.scale_;

        if(!rhs.components_.empty())
        {
            Node components;
            for (const auto &component : rhs.components_)
            {
                const Node &componentNode = YAML::convert<ComponentData>::encode(*component);
                components.push_back(componentNode);
            }
            node["components"] = components;
        }

        if(!rhs.children_.empty())
        {
            Node children;
            for (const auto &child : rhs.children_)
            {
                const Node &childNode = YAML::convert<ObjectData>::encode(*child);
                children.push_back(childNode);
            }
            node["children"] = children;
        }

        return node;
    }

    static bool decode(const Node &node, ObjectData &rhs) {
        rhs.name_ = node["name"].as<std::string>("");
        rhs.position_ = node["position"].as<Vector2DData>(Vector2DData());
        rhs.rotation_ = node["rotation"].as<float>(0);
        Vector2DData scale;
        scale.xy[0] = 1;
        scale.xy[1] = 1;
        rhs.scale_ = node["scale"].as<Vector2DData>(scale);

        const Node &componentsNode = node["components"];
        if(componentsNode.IsDefined() && componentsNode.IsSequence())
        {
            for(auto it = componentsNode.begin(); it != componentsNode.end(); ++it)
            {
                ComponentDataRef componentData = std::make_shared<ComponentData>();
                bool converted = YAML::convert<ComponentData>::decode(*it, *componentData);
                if(!converted)
                    return false;
                rhs.components_.push_back(componentData);
            }
        }

        const Node &childrenNode = node["children"];
        if(childrenNode.IsDefined() && childrenNode.IsSequence())
        {
            for(auto it = childrenNode.begin(); it != childrenNode.end(); ++it)
            {
                ObjectDataRef childData = std::make_shared<ObjectData>();
                bool converted = YAML::convert<ObjectData>::decode(*it, *childData);
                if(!converted)
                    return false;
                rhs.children_.push_back(childData);
            }
        }
        return true;
    }
};

template<>
struct convert<PrototypeReference> {
    static Node encode(const PrototypeReference &rhs) {
        Node node;
        node["prototype"] = rhs.prototype_;
        node["name"] = rhs.name_;
        node["position"] = rhs.getPosition();
        node["rotation"] = rhs.getRotation();
        node["scale"] = rhs.getScale();

        return node;
    }

    static bool decode(const Node &node, PrototypeReference &rhs) {
        rhs.prototype_ = node["prototype"].as<std::string>();
        rhs.name_ = node["name"].as<std::string>("");
        rhs.setPosition(node["position"].as<Vector2DData>(Vector2DData()));
        rhs.setRotation(node["rotation"].as<float>(0.f));
        Vector2DData scale;
        scale.xy[0] = 1;
        scale.xy[1] = 1;
        rhs.setScale(node["scale"].as<Vector2DData>(scale));

        return true;
    }
};

template<>
struct convert<ProjectData> {
    static Node encode(const ProjectData &rhs) {
        Node node;
        node["name"] = rhs.folderName_;
        node["lastSceneEdited"] = rhs.currentScenePath_;
        return node;
    }

    static bool decode(const Node &node, ProjectData &rhs) {
        rhs.folderName_ = node["name"].as<std::string>();
        const Node &lastSceneNode = node["lastSceneEdited"];
        if(lastSceneNode)
            rhs.currentScenePath_ = lastSceneNode.as<std::string>("");
        return true;
    }
};

template <>
struct convert<SceneData> {
    static Node encode(const SceneData &rhs) {
        Node node;
        node["name"] = rhs.name_;
        Node objectsNode;


        for(auto i = 0; i < rhs.objectsSize(); i++)
        {
            objectsNode.push_back(*rhs.getObject(i).get());
        }

        if(objectsNode.size())
            node["prototypes"] = objectsNode;

        return node;
    }

    static bool decode(const Node &node, SceneData &rhs) {
        rhs.name_ = node["name"].as<std::string>();
        const Node &objectsNode = node["prototypes"];
        if(objectsNode)

        for(auto i = 0; i < objectsNode.size();i++)
        {
            auto object = objectsNode[i].as<PrototypeReference>();
            rhs.addObject(std::make_shared<PrototypeReference>(object));
        }
        // TODO: load objects
        return true;
    }
};
}

#endif //GAMEENGINEEDITOR_VIEWMODELS_HPP
