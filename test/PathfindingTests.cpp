#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <game-engine/pathfinding/AStarPathfinding.hpp>

using namespace GameEngine;
using namespace ::testing;

// All test use a 5x5 grid to simplify
// +---+---+---+---+---+
// |0,0|1,0|2,0|3,0|4,0|
// +---+---+---+---+---+
// |0,1|1,1|2,1|3,1|4,1|
// +---+---+---+---+---+
// |0,2|1,2|2,2|3,2|4,2|
// +---+---+---+---+---+
// |0,3|1,3|2,3|3,3|4,3|
// +---+---+---+---+---+
// |0,4|1,4|2,4|3,4|4,4|
// +---+---+---+---+---+



void testPath(const std::vector<Vec2D> &expected)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5);

    const Path &path = pathfinder_->findPath(expected[0], expected[expected.size()-1]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expected));
}

void testPathWithModifiers(const std::vector<Vec2D> &expected, const Vec2D &minModifierPos, const Vec2D &maxModifierPos, int modifier)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5);

    pathfinder_->setModifier(minModifierPos, maxModifierPos, modifier);
    const Path &path = pathfinder_->findPath(expected[0], expected[expected.size()-1]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expected));
}

void testPathWithAgents(const std::vector<Vec2D> &expected, const std::vector<Vec2D> &agentsPos)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5);

    for(auto pos : agentsPos)
    {
        AgentRef agent = std::make_shared<Agent>();
        agent->setPosition(pos);
        pathfinder_->addAgent(agent);
    }

    const Path &path = pathfinder_->findPath(expected[0], expected[expected.size()-1]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expected));
}

TEST(Pathfinding, crossRightLeftAndVerticalCenter)
{
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |OO|--|--|--|DD|
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,2),
        Vec2D(3,2),
        Vec2D(4,2)
    };
    testPath(expectedPath);
}


TEST(Pathfinding, crossUpDownAndHorizontalCenter)
{
//  +--+--+--+--+--+
//  |  |  |OO|  |  |
//  +--+--+--+--+--+
//  |  |  ||||  |  |
//  +--+--+--+--+--+
//  |  |  ||||  |  |
//  +--+--+--+--+--+
//  |  |  ||||  |  |
//  +--+--+--+--+--+
//  |  |  |DD|  |  |
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(2,0),
        Vec2D(2,1),
        Vec2D(2,2),
        Vec2D(2,3),
        Vec2D(2,4)
    };

    testPath(expectedPath);
}

TEST(Pathfinding, FromUpperLeftToBottomRight)
{
//  +--+--+--+--+--+
//  |OO|  |  |  |  |
//  +--+--+--+--+--+
//  |  |\\|  |  |  |
//  +--+--+--+--+--+
//  |  |  |\\|  |  |
//  +--+--+--+--+--+
//  |  |  |  |\\|  |
//  +--+--+--+--+--+
//  |  |  |  |  |DD|
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(0,0),
        Vec2D(1,1),
        Vec2D(2,2),
        Vec2D(3,3),
        Vec2D(4,4)
    };

    testPath(expectedPath);
}


TEST(Pathfinding, FromUpperRightToBottomLeft)
{
//  +--+--+--+--+--+
//  |  |  |  |  |OO|
//  +--+--+--+--+--+
//  |  |  |  |//|  |
//  +--+--+--+--+--+
//  |  |  |//|  |  |
//  +--+--+--+--+--+
//  |  |//|  |  |  |
//  +--+--+--+--+--+
//  |DD|  |  |  |  |
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(4,0),
        Vec2D(3,1),
        Vec2D(2,2),
        Vec2D(1,3),
        Vec2D(0,4)
    };

    testPath(expectedPath);
}

TEST(Pathfinding, crossRightLeftAndVerticalCenterMiddleModified)
{
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |--|  |  |
//  +--+--+--+--+--+
//  |OO|//|MM|\\|DD|
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,1),
        Vec2D(3,2),
        Vec2D(4,2)
    };
    testPathWithModifiers(expectedPath,Vec2D(2,2),Vec2D(2,2),10);
}


TEST(Pathfinding, crossRightLeftPlusOneAndVerticalCenterMiddleModified)
{
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |--|  |  |
//  +--+--+--+--+--+
//  |OO|//|MM|\\|  |
//  +--+--+--+--+--+
//  |  |  |MM|  |DD|
//  +--+--+--+--+--+
//  |  |  |MM|  |  |
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,1),
        Vec2D(3,2),
        Vec2D(4,3)
    };
    testPathWithModifiers(expectedPath,Vec2D(2,2),Vec2D(2,4),10);
}

TEST(Pathfinding, checkAgentsBlockCells)
{
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |--|--|  |
//  +--+--+--+--+--+
//  |  |//|AA|AA|\\|
//  +--+--+--+--+--+
//  |OO|  |AA|AA|DD|
//  +--+--+--+--+--+

    std::vector<Vec2D> expectedPath = {
        Vec2D(0,4),
        Vec2D(1,3),
        Vec2D(2,2),
        Vec2D(3,2),
        Vec2D(4,3),
        Vec2D(4,4)
    };

    std::vector<Vec2D> agents = {
        Vec2D(2,4),
        Vec2D(2,3),
        Vec2D(3,3),
        Vec2D(3,4)
    };
    testPathWithAgents(expectedPath, agents);
}

TEST(Pathfinding, checkAgentMovingBlockCells)
{
//  1st path
//  +--+--+--+--+--+
//  |AA|  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |OO|--|--|--|DD|
//  +--+--+--+--+--+
    std::vector<Vec2D> expectedFirstPath = {
        Vec2D(0,4),
        Vec2D(1,4),
        Vec2D(2,4),
        Vec2D(3,4),
        Vec2D(4,4)
    };
//  2nd path
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |  |  |
//  +--+--+--+--+--+
//  |  |  |  |--|  |
//  +--+--+--+--+--+
//  |OO|--|//|AA|DD|
//  +--+--+--+--+--+
    std::vector<Vec2D> expectedSecondPath = {
        Vec2D(0,4),
        Vec2D(1,4),
        Vec2D(2,4),
        Vec2D(3,3),
        Vec2D(4,4)
    };

    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5);
    AgentRef agent = std::make_shared<Agent>();
    agent->setPosition(Vec2D(0,0));
    pathfinder_->addAgent(agent);

    // check first path
    const Path &firstPath = pathfinder_->findPath(expectedFirstPath[0], expectedFirstPath[4]);
    EXPECT_THAT(firstPath.cells_, ElementsAreArray(expectedFirstPath));

    // check second path
    agent->setPosition(Vec2D(3,4));
    const Path &secondPath = pathfinder_->findPath(expectedSecondPath[0], expectedSecondPath[4]);
    EXPECT_THAT(secondPath.cells_, ElementsAreArray(expectedSecondPath));
}
