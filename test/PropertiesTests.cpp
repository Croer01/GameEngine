#include "gtest/gtest.h"
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/properties/Property.hpp>
#include <thread>
#include <chrono>

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