//
// Created by adria on 22/04/2020.
//

#include "Path.hpp"

namespace GameEngine {
namespace Internal {

Path::Path()
{
}

Path::Path(const CellRef &origin, const CellRef &destination)
{
    CellRef next = destination;
    cells_.emplace_back(origin->x, origin->y);
    while (next != nullptr && next != origin)
    {
        cells_.insert(cells_.begin()+1,Vec2D(next->x, next->y));
        if(auto parent = next->parent.lock())
            next = parent;
        else
            next = nullptr;
    }
}

}
}
