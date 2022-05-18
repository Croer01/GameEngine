//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_CELL_HPP
#define GAMEENGINE_CELL_HPP

#include <memory>
#include <game-engine/api.hpp>

namespace GameEngine {

class Cell;
typedef std::shared_ptr<Cell> CellRef;

class PUBLICAPI Cell
{
public:
    Cell(int x, int y);
    // XY position
    int x;
    int y;
    // this is a constant modifier used to set extra weight in the cost calculation
    int modifier;
    // accumulated cost to move to this cell respect the origin
    int g;
    // contains the G value and add the heuristic cost
    int f;
    // a reference to the previous cell to be able to the previous cell that could chain to form a path
    std::weak_ptr<Cell> parent;
    bool blocked;
};

}

#endif //GAMEENGINE_CELL_HPP
