//
// Created by adria on 22/04/2020.
//

#include <stdexcept>
#include <cassert>
#include "AStarPathfinding.hpp"

namespace GameEngine {
namespace Internal {

AStarPathfinding::AStarPathfinding(int width, int height) :
    width_(width), height_(height)
{
    grid_ = std::vector<std::vector<CellRef>>(height_);

    for(int y = 0; y < height; y++)
    {
        std::vector<CellRef> row(width);
        for(int x = 0; x < width; x++)
        {
            row[x] = std::make_shared<Cell>(x,y);
        }
        grid_[y].swap(row);
    }
}

void AStarPathfinding::setModifier(const Vec2D &minPos, const Vec2D &maxPos, int modifierValue)
{
    if(minPos.x > maxPos.x)
        throw std::invalid_argument("min X is greater than max X");
    if(minPos.y > maxPos.y)
        throw std::invalid_argument("min Y is greater than max Y");

    if(minPos == maxPos)
    {
        grid_[minPos.y][minPos.x]->modifier = modifierValue;
    }
    else
    {
        for (int y = minPos.y; y <= maxPos.y; y++)
        {
            std::vector<CellRef> row = grid_[y];
            for (int x = minPos.x; x <= maxPos.x; x++)
            {
                row[x]->modifier = modifierValue;
            }
        }
    }
}

Path AStarPathfinding::findPath(const Vec2D &initialPos, const Vec2D &targetPos)
{
    std::vector<CellRef> openList;
    std::vector<CellRef> closedList;
    CellRef firstCell = grid_[initialPos.y][initialPos.x];
    firstCell->g = 0;
    firstCell->f = 0;
    openList.push_back(firstCell);
    CellRef destination = grid_[targetPos.y][targetPos.x];
    while (!openList.empty()) {
        //TODO: use forward_list? probably YES!

        // get the best cell, aka lowest F value
        auto best = std::min_element(openList.begin(), openList.end(),
            [](const CellRef &a, const CellRef &b) { return a->f < b->f; });
        assert(best != openList.end());
        CellRef current = *best;
        openList.erase(best);
        closedList.push_back(current);

        if (current == destination)
            return Path(firstCell, current);

        for(const CellRef& neighbor : findNeighborCells(current)) {

            int neighborG = calculateGScore (current, neighbor);
            int H = calculateHeuristic (neighbor, destination);
            int neighborF = neighborG + H;

            bool closedContains = std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end();
            bool openContains = std::find(openList.begin(), openList.end(), neighbor) != openList.end();
            if(!closedContains && (!openContains || neighborF < neighbor->f))
            {
                neighbor->g = neighborG;
                neighbor->f = neighborF;
                neighbor->parent = current;

                if (!openContains)
                    openList.push_back(neighbor);
            }
        }
    }

    return Path();
}

std::vector<CellRef> AStarPathfinding::findNeighborCells(const CellRef &cell)
{
    std::vector<CellRef> neighbors;

    for(auto y = cell->y - 1; y <= cell->y+1; y++)
    {
        if(y < 0 || y >= height_)
            continue;

        std::vector<CellRef> &row = grid_[y];
        for(auto x = cell->x - 1; x <= cell->x+1; x++)
        {
            if(x < 0 || x >= width_ || (x == cell->x && y == cell->y))
                continue;

            neighbors.push_back(row[x]);
        }
    }

    return neighbors;
}

int AStarPathfinding::calculateGScore(const CellRef &origin, const CellRef &destination)
{
    if (destination->blocked)
    {
        constexpr int highValue = 1000000;
        return highValue;
    }
    else
        return origin->g + destination->modifier + 1;// always have a minimum cost of 1 step
}

int AStarPathfinding::calculateHeuristic(const CellRef &origin, const CellRef &destination)
{
    if (origin->blocked)
    {
        constexpr int highValue = 1000000;
        return highValue;
    }
    else
    {
        Vec2D originPoint(origin->x, origin->y);
        Vec2D destinationPoint(destination->x, destination->y);

        return std::abs((destinationPoint - originPoint).sqrMagnitude()) + origin->modifier * origin->modifier;
    }
}

void AStarPathfinding::addAgent(const AgentRef &agent)
{
    //TODO: calcular la posicio de la cel·la i posar-le com a blocada
    const Vec2D &position = agent->getPosition();
    grid_[position.y][position.x]->blocked = true;
    agent->registerObserver(this);
    agents_.push_back(agent);
}

void AStarPathfinding::onEvent(const Subject<AgentEvents> &target, const AgentEvents &event, void *args)
{
    const Agent &agent = dynamic_cast<const Agent &>(target);
    Vec2D prevPos = *(Vec2D*)args;

    grid_[prevPos.y][prevPos.x]->blocked = false;
    grid_[agent.getPosition().y][agent.getPosition().x]->blocked = true;
}

}
}
