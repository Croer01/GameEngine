//
// Created by adria on 22/04/2020.
//

#include <game-engine/pathfinding/Cell.hpp>

namespace GameEngine
{

Cell::Cell(int x, int y) :
    x(x),
    y(y),
    modifier(0),
    f(0),
    g(0),
    blocked(false)
{}

}