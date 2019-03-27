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