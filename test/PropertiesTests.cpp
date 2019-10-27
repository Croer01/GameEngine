#include "gtest/gtest.h"
#include "../private/Data.hpp"
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/properties/Property.hpp>
#include <thread>
#include <chrono>
#include <game-engine/geIO.hpp>

class TestData : public GameEngine::PropertiesHolder<TestData>{
    int privateIntValue_;
public:
    int getPrivate() const { return privateIntValue_; };
    void setPrivate(const int &value){ privateIntValue_ = value; };
private:
    GameEngine::PropertySetBase *instantiateProperties() override
    {
        auto properties = new GameEngine::PropertySet<TestData>();

        return properties;
    }
};

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

GameEngine::PropertySet<TestData> *createPropertySet(TestData &target, int privateFieldValue, GameEngine::PropertySet<TestData> *parent)
{
    GameEngine::PropertySet<TestData> *set = nullptr;
    if(parent)
    {
       set = new GameEngine::PropertySet<TestData>(parent);
    }
    else
    {
        set = new GameEngine::PropertySet<TestData>();
    }

    auto property = new GameEngine::Property<TestData, int>("value", &TestData::getPrivate, &TestData::setPrivate,0);
    property->set(&target, privateFieldValue);
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
    EXPECT_EQ(instance.getPrivate(), instanceProp.get(&instance));
    EXPECT_EQ(parent.getPrivate(), parentProp.get(&parent));

    // prepare copy with the same struct but different targets
    TestData parentCopy;
    TestData instanceCopy;
    auto parentPropSetCopy = new GameEngine::PropertySet<TestData>();
    auto instancePropSetCopy = new GameEngine::PropertySet<TestData>(parentPropSetCopy);

    // Do the copy and check all works fine
    instancePropSet->copy(std::shared_ptr<TestData>(&instance),std::shared_ptr<TestData>(&instanceCopy));

    auto &instancePropCopy = dynamic_cast<GameEngine::Property<TestData, int>&>(instancePropSetCopy->get(0));
    auto &parentPropCopy = dynamic_cast<GameEngine::Property<TestData, int>&>(parentPropSetCopy->get(0));

    EXPECT_NE(instanceCopy.getPrivate(), parentCopy.getPrivate());
    EXPECT_EQ(instanceCopy.getPrivate(), instancePropCopy.get(&instanceCopy));
    EXPECT_EQ(parentCopy.getPrivate(), parentPropCopy.get(&parentCopy));
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