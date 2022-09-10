//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_PATH_HPP
#define GAMEENGINE_PATH_HPP


#include <game-engine/api.hpp>
#include <vector>
#include <game-engine/pathfinding/Cell.hpp>

namespace GameEngine {

class PUBLICAPI Path
{
public:
    std::vector<Vec2D> cells_;
    Path();
    Path(const CellRef &origin, const CellRef &destination);
};

}


#endif //GAMEENGINE_PATH_HPP
