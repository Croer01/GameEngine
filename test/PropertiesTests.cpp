#include "gtest/gtest.h"
#include "../private/Data.hpp"
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/properties/Property.hpp>
#include <thread>
#include <chrono>
#include <game-engine/geIO.hpp>

struct TestData{
private:
    int privateIntValue_;
public:
    int intValue;

    int getPrivate() const { return privateIntValue_; };
    void setPrivate(const int &value){ privateIntValue_ = value; };
};

TEST(Properties, setValueFromProperty)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.intValue = defaultValue;
    GameEngine::Property<TestData, int> property("value",&instance,&TestData::intValue,0);
    EXPECT_EQ(instance.intValue, property.get());
    property.set(newValue);
    EXPECT_EQ(instance.intValue, newValue);
    EXPECT_EQ(instance.intValue, property.get());
}

TEST(Properties, getValueFromPropertyEdited)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.intValue = defaultValue;
    GameEngine::Property<TestData, int> property("value",&instance,&TestData::intValue,0);
    EXPECT_EQ(instance.intValue, property.get());
    instance.intValue = newValue;
    EXPECT_EQ(property.get(), newValue);
    EXPECT_EQ(instance.intValue, property.get());
}


TEST(Properties, setValueFromPropertySetter)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.setPrivate(defaultValue);
    GameEngine::Property<TestData, int> property("value",&instance,&TestData::getPrivate,&TestData::setPrivate,0);
    EXPECT_EQ(instance.getPrivate(), property.get());
    property.set(newValue);
    EXPECT_EQ(instance.getPrivate(), newValue);
    EXPECT_EQ(instance.getPrivate(), property.get());
}

TEST(Properties, getValueFromPropertyGetterAndEdited)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.setPrivate(defaultValue);
    GameEngine::Property<TestData, int> property("value",&instance,&TestData::getPrivate,&TestData::setPrivate,0);
    EXPECT_EQ(instance.getPrivate(), property.get());
    property.set(newValue);
    EXPECT_EQ(property.get(), newValue);
    EXPECT_EQ(instance.getPrivate(), property.get());
}

GameEngine::PropertySet<TestData> *createPropertySet(TestData &target, int privateFieldValue, GameEngine::PropertySet<TestData> *parent)
{
    GameEngine::PropertySet<TestData> *set = nullptr;
    if(parent)
    {
       set = new GameEngine::PropertySet<TestData>(&target, parent);
    }
    else
    {
        set = new GameEngine::PropertySet<TestData>(&target);
    }

    auto property = new GameEngine::Property<TestData, int>("value",&target,&TestData::getPrivate,&TestData::setPrivate,0);
    property->set(privateFieldValue);
    set->add(property);
    return set;
}

TEST(Properties, copyPropertyTree)
{
    TestData parent;
    TestData instance;

    // cretae the original data structure
    auto parentPropSet = createPropertySet(parent, 5, nullptr);
    auto instancePropSet = createPropertySet(instance, 10, parentPropSet);

    auto &instanceProp = dynamic_cast<GameEngine::Property<TestData, int>&>(instancePropSet->get(0));
    auto &parentProp = dynamic_cast<GameEngine::Property<TestData, int>&>(parentPropSet->get(0));

    EXPECT_NE(instance.getPrivate(), parent.getPrivate());
    EXPECT_EQ(instance.getPrivate(), instanceProp.get());
    EXPECT_EQ(parent.getPrivate(), parentProp.get());

    // prepare copy with the same struct but different targets
    TestData parentCopy;
    TestData instanceCopy;
    auto parentPropSetCopy = new GameEngine::PropertySet<TestData>(&parentCopy);
    auto instancePropSetCopy = new GameEngine::PropertySet<TestData>(&instanceCopy, parentPropSetCopy);

    // Do the copy and check all works fine
    instancePropSet->copy(*instancePropSetCopy);

    auto &instancePropCopy = dynamic_cast<GameEngine::Property<TestData, int>&>(instancePropSetCopy->get(0));
    auto &parentPropCopy = dynamic_cast<GameEngine::Property<TestData, int>&>(parentPropSetCopy->get(0));

    EXPECT_NE(instanceCopy.getPrivate(), parentCopy.getPrivate());
    EXPECT_EQ(instanceCopy.getPrivate(), instancePropCopy.get());
    EXPECT_EQ(parentCopy.getPrivate(), parentPropCopy.get());
}

TEST(Data, writeValuesIntoData)
{
    const YAML::Node &node = YAML::Node();
    const auto &data = std::make_shared<GameEngine::Internal::Data>(node);
    
    GameEngine::geDataWriter writer(data);

    EXPECT_EQ(data->hasValue("float"), false);
    
    writer.writeFloat("float",2.5f);
    EXPECT_EQ(data->hasValue("float"), true);
    EXPECT_EQ(data->getFloat("float"), 2.5f);
    
}