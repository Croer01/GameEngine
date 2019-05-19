#include <game-engine/api.hpp>
#include "gtest/gtest.h"
#include <ostream>

std::ostream& operator<<(std::ostream& os, const GameEngine::Vec2D& vec) {
    return os << "{ x:" << std::to_string(vec.x) << ", y:" << std::to_string(vec.y) << " }";
}

class VectorTest : public testing::TestWithParam<std::pair<GameEngine::Vec2D,GameEngine::Vec2D>> {};

TEST_P(VectorTest, NormalizeVector)
{
    GameEngine::Vec2D unitVector = GetParam().first;
    unitVector.normalize();

    EXPECT_FLOAT_EQ(unitVector.x, GetParam().second.x);
    EXPECT_FLOAT_EQ(unitVector.y, GetParam().second.y);
}

INSTANTIATE_TEST_CASE_P(InstantiationName, VectorTest, ::testing::Values(
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({5,0},{1,0}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({0,2},{0,1}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({0,0},{0,0}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({10,10},{0.70710677f,0.70710677f}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({-5.5f,0},{-1,0}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({0,-2.4f},{0,-1}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({0,0},{0,0}),
        std::make_pair<GameEngine::Vec2D,GameEngine::Vec2D>({-3,3},{-0.70710683f,0.70710683f})
));