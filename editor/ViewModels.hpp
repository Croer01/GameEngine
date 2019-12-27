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
    COLOR
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
    Vector2DData rotation_;
    std::string name_;
    std::vector<ObjectDataRef> children_;
    std::vector<ComponentDataRef> components_;
};

class SceneData
{
public:
    std::string name_;
    std::vector<ObjectDataRef> objects_;
    std::string filePath_;
};

class ProjectData
{
public:
    std::string folderPath_;
    std::string folderName_;
    std::string currentScenePath_;
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
struct convert<ObjectData> {
    static Node encode(const ObjectData &rhs) {
        Node node;
        node["name"] = rhs.name_;
        node["position"] = rhs.position_;
        node["rotation"] = rhs.rotation_;
        node["scale"] = rhs.scale_;
        return node;
    }

    static bool decode(const Node &node, ObjectData &rhs) {
        rhs.name_ = node["name"].as<std::string>();
        rhs.position_ = node["position"].as<Vector2DData>();
        rhs.rotation_ = node["rotation"].as<Vector2DData>();
        rhs.scale_ = node["scale"].as<Vector2DData>();
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
            rhs.currentScenePath_ = lastSceneNode.as<std::string>();
        return true;
    }
};

template <>
struct convert<SceneData> {
    static Node encode(const SceneData &rhs) {
        Node node;
        node["name"] = rhs.name_;
        Node objectsNode;

        for(const ObjectDataRef &object : rhs.objects_){
            objectsNode.push_back(*object.get());
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
            auto object = objectsNode[i].as<ObjectData>();
            rhs.objects_.push_back(std::make_shared<ObjectData>(object));
        }
        // TODO: load objects
        return true;
    }
};
}

#endif //GAMEENGINEEDITOR_VIEWMODELS_HPP
