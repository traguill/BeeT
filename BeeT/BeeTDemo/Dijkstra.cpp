#include "Dijkstra.h"

#include <list>

using namespace std;

Dijkstra::Dijkstra()
{
	for (int x = 0; x < NUM_TILES_X; x++)
		for (int y = 0; y < NUM_TILES_Y; y++)
			isWalkable[x][y] = true;
	
}

Dijkstra::~Dijkstra()
{}

void Dijkstra::SetWalkableMap(const std::vector<iPoint>& walkableTiles)
{
	for (auto tile : walkableTiles)
	{
		isWalkable[tile.x][tile.y] = true;
	}
}

bool Dijkstra::FindPath(const iPoint& tileStart, const iPoint & tileEnd, vector<iPoint>& path) const
{
	if (!isWalkable[tileStart.x][tileStart.y] || !isWalkable[tileEnd.x][tileEnd.y])
		return false;

	path.clear();

	DNode grid[NUM_TILES_X][NUM_TILES_Y];
	list<DNode*> toVisit;
	toVisit.push_back(&grid[tileStart.x][tileStart.y]);
	toVisit.front()->pos = tileStart;
	DNode* current = nullptr;
	bool found = false;
	while (!found && toVisit.size() > 0)
	{
		current = toVisit.front();
		toVisit.pop_front();

		// Is end?
		if (current->pos == tileEnd)
		{
			current->dst = current->parent->dst + 1;
			found = true;
			continue;
		}

		// Calculate dst to origin
		if (current->parent == nullptr) // Start node
			current->dst = 0;
		else
			current->dst = current->parent->dst + 1;

		current->visited = true;
		// push all neighbours
		for (int x = current->pos.x -1; x <= current->pos.x + 1; x++)
		{
			for (int y = current->pos.y -1; y <= current->pos.y + 1; y++)
			{
				if (isWalkable[x][y] && grid[x][y].visited == false)
				{
					DNode* child = &grid[x][y];
					child->parent = current;
					child->pos = iPoint(x, y);
					toVisit.push_back(child);
				}
			}
		}
	}

	if (found)
	{
		// Current is now end tile and dst is number of nodes of the path
		path.resize(current->dst);
		for (int i = current->dst - 1; i >= 0; i--)
		{
			path[i] = current->pos;
			current = current->parent;
		}
	}

	return found;
}
