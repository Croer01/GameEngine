#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../private/pathfinding/AStarPathfinding.hpp"

using namespace GameEngine;
using namespace GameEngine::Internal;
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
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5,1);

    const Path &path = pathfinder_->findPath(expected[0], expected[4]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expected));
}

void testPathWithModifiers(const std::vector<Vec2D> &expected, const Vec2D &minModifierPos, const Vec2D &maxModifierPos, int modifier)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5,1);

    pathfinder_->setModifier(minModifierPos, maxModifierPos, modifier);
    const Path &path = pathfinder_->findPath(expected[0], expected[4]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expected));
}

TEST(Pathfinding, crossRightLeftAndVerticalCenter)
{
    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,2),
        Vec2D(3,2),
        Vec2D(4,2),
    };
    testPath(expectedPath);
}


TEST(Pathfinding, crossUpDownAndHorizontalCenter)
{
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
    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,1),
        Vec2D(3,2),
        Vec2D(4,2),
    };
    testPathWithModifiers(expectedPath,Vec2D(2,2),Vec2D(2,2),10);
}


TEST(Pathfinding, crossRightLeftPlusOneAndVerticalCenterMiddleModified)
{
    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,1),
        Vec2D(3,2),
        Vec2D(4,3),
    };
    testPathWithModifiers(expectedPath,Vec2D(2,2),Vec2D(2,4),10);
}
