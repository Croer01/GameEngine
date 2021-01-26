//
// Created by adria on 22/04/2020.
//

#ifndef GAMEENGINE_PATH_HPP
#define GAMEENGINE_PATH_HPP


#include <game-engine/api.hpp>
#include <vector>
#include <game-engine/internal/pathfinding/Cell.hpp>

namespace GameEngine {
namespace Internal {

class Path
{
public:
    std::vector<Vec2D> cells_;
    Path();
    Path(const CellRef &origin, const CellRef &destination);
};

}
}


#endif //GAMEENGINE_PATH_HPP
