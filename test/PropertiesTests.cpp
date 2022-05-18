#include <gtest/gtest.h>
//#include <game-engine/internal/Data.hpp>
#include <game-engine/components/ComponentData.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/geIO.hpp>

using namespace GameEngine;

class TestData : public GameEngine::ComponentData {
public:
    TestData()
    {
        createProperty<int>("value",0);
    }
};

class TestDataChild : public TestData{
public:
    TestDataChild() : TestData()
    {
        createProperty<int>("childValue", 1);
    }
};

class TestComponent : public GameEngine::geComponentInstantiable<TestComponent, TestData>
{
};

TEST(Properties, setValueFromPropertySetter)
{
    int defaultValue = -1;
    int newValue = 10;
    auto component = std::make_shared<TestComponent>();
    const ComponentDataRef &instance = component->instantiateData();
    Property<int> *property = instance->getProperty<int>("value");
    component->setData(instance);
    // check set value from component to path
    component->setPropertyValue<int>("value", defaultValue);
    EXPECT_EQ(component->getPropertyValue<int>("value"), property->get());

    // check set value from property directly and component get the new value
    property->set(newValue);
    EXPECT_EQ(component->getPropertyValue<int>("value"), newValue);
    EXPECT_EQ(component->getPropertyValue<int>("value"), property->get());
}

TEST(Properties, copyProperties)
{
    TestDataChild testDataChild;

    // Check has own property and the inherited from TestData
    EXPECT_EQ(testDataChild.getProperty<int>("value")->get(), 0);
    EXPECT_EQ(testDataChild.getProperty<int>("childValue")->get(), 1);

    // Change the default values and clone to check all properties are set correctly
    testDataChild.getProperty<int>("value")->set(10);
    testDataChild.getProperty<int>("childValue")->set(11);
    auto clone = std::dynamic_pointer_cast<TestDataChild>(testDataChild.clone<TestDataChild>());
    EXPECT_EQ(testDataChild.getProperty<int>("value")->get(), clone->getProperty<int>("value")->get());
    EXPECT_EQ(testDataChild.getProperty<int>("childValue")->get(), clone->getProperty<int>("childValue")->get());
}

TEST(Properties, CallObserverOnValueChange)
{
    TestDataChild testDataChild;

    int callCounter = 0;
    Property<int> *prop = testDataChild.getProperty<int>("childValue");
    prop->registerObserver([&callCounter](){
       callCounter++; 
    });

    prop->set(1);
    EXPECT_EQ(callCounter, 1);
    prop->set(0);
    EXPECT_EQ(callCounter, 2);
    prop->set(1);
    EXPECT_EQ(callCounter, 3);
}

TEST(Properties, throwExcpetionUsingWrongGetters)
{
    TestDataChild testDataChild;

    EXPECT_NO_THROW(
        testDataChild.getProperty<int>("childValue");
    );

    EXPECT_ANY_THROW(
        testDataChild.getEnumProperty("childValue");
    );

    EXPECT_ANY_THROW(
        testDataChild.getFilePathProperty("childValue");
    );
}

// TODO: Reenable this test after organize all the code to build with meson
/*
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
*/