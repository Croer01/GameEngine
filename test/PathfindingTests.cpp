#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../private/pathfinding/AStarPathfinding.hpp"

using namespace GameEngine;
using namespace GameEngine::Internal;
using namespace ::testing;

TEST(Pathfinding, crossRightLeftAndVerticalCenter)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5,1);
    std::vector<Vec2D> expectedPath = {
        Vec2D(0,2),
        Vec2D(1,2),
        Vec2D(2,2),
        Vec2D(3,2),
        Vec2D(4,2),
    };

    const Path &path = pathfinder_->findPath(expectedPath[0], expectedPath[4]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expectedPath));
}


TEST(Pathfinding, crossUpDownAndHorizontalCenter)
{
    std::unique_ptr<AStarPathfinding> pathfinder_ =std::make_unique<AStarPathfinding>(5,5,1);
    std::vector<Vec2D> expectedPath = {
        Vec2D(2,0),
        Vec2D(2,1),
        Vec2D(2,2),
        Vec2D(2,3),
        Vec2D(2,4)
    };

    const Path &path = pathfinder_->findPath(expectedPath[0], expectedPath[4]);
    EXPECT_THAT(path.cells_, ElementsAreArray(expectedPath));
}
