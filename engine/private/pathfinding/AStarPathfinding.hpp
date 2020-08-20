//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_ASTARPATHFINDING_HPP
#define GAMEENGINE_ASTARPATHFINDING_HPP


#include <vector>
#include <game-engine/api.hpp>
#include "Cell.hpp"
#include "Agent.hpp"
#include "Path.hpp"

namespace GameEngine {
namespace Internal {

// The implementation of the algorithm is based on https://www.redblobgames.com/pathfinding/a-star/introduction.html
class AStarPathfinding : public Observer<AgentEvents>
{
    int width_;
    int height_;
    std::vector<std::vector<CellRef>> grid_;
    std::vector<AgentRef> agents_;

    std::vector<CellRef> findNeighborCells (const CellRef &cell);
    int calculateGScore (const CellRef &origin, const CellRef &destination);
    int calculateHeuristic (const CellRef &origin, const CellRef &destination);
public:
    AStarPathfinding(int width, int height);
    //TODO: (maybe a helper class?)
    void setModifier(const Vec2D &minPos, const Vec2D &maxPos, int modifierValue);
    void addAgent(const AgentRef &agent);
    Path findPath(const Vec2D &initialPos, const Vec2D &targetPos);
    virtual void onEvent(const Subject<AgentEvents> &target, const AgentEvents &event, void *args);
    void removeAgent(const AgentRef &agent);
};

}

}
#endif //GAMEENGINE_ASTARPATHFINDING_HPP
