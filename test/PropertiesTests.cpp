#include "gtest/gtest.h"
#include "../private/Data.hpp"
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/properties/Property.hpp>
#include <game-engine/properties/PropertiesRegister.hpp>
#include <thread>
#include <chrono>
#include <game-engine/geIO.hpp>

class TestData : public GameEngine::PropertiesHolder<TestData>{
    int privateIntValue_;
public:
    int getPrivate() const { return privateIntValue_; };
    void setPrivate(const int &value){ privateIntValue_ = value; };
};

class TestDataChild : public TestData{
};

namespace GameEngine {
template <>
struct PropertyInstantiator<TestData>
{
    static PropertySetBase* instantiate()
    {
        auto properties = new GameEngine::PropertySet<TestData>();
        properties->add(new GameEngine::Property<TestData, int>(
            "value",
            &TestData::getPrivate,
            &TestData::setPrivate,
            0)
        );

        return properties;
    }
};
}

TEST(Properties, setValueFromPropertySetter)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.setPrivate(defaultValue);
    GameEngine::Property<TestData, int> property("value", &TestData::getPrivate, &TestData::setPrivate,0);
    EXPECT_EQ(instance.getPrivate(), property.get(&instance));
    property.set(&instance, newValue);
    EXPECT_EQ(instance.getPrivate(), newValue);
    EXPECT_EQ(instance.getPrivate(), property.get(&instance));
}

TEST(Properties, getValueFromPropertyGetterAndEdited)
{
    int defaultValue = -1;
    int newValue = 10;
    TestData instance;
    instance.setPrivate(defaultValue);
    GameEngine::Property<TestData, int> property("value", &TestData::getPrivate, &TestData::setPrivate,0);
    EXPECT_EQ(instance.getPrivate(), property.get(&instance));
    property.set(&instance, newValue);
    EXPECT_EQ(property.get(&instance), newValue);
    EXPECT_EQ(instance.getPrivate(), property.get(&instance));
}

TEST(Properties, copyPropertyTree)
{
    std::shared_ptr<TestDataChild> instance(new TestDataChild());

    // cretae the original data structure
    auto instancePropSet = std::dynamic_pointer_cast<GameEngine::PropertySet<TestData>>(TestData::getProperties().lock());

    auto &instanceProp = dynamic_cast<GameEngine::Property<TestData, int>&>(instancePropSet->get(0));

    EXPECT_EQ(instance->getPrivate(), instanceProp.get(instance.get()));

    // prepare copy with the same struct but different targets
    std::shared_ptr<TestDataChild> instanceCopy(new TestDataChild());

    // Do the copy and check all works fine
    instancePropSet->copy(instance, instanceCopy);

    EXPECT_EQ(instanceCopy->getPrivate(), instanceProp.get(instanceCopy.get()));
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