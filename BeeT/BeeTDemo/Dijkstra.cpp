#include "Dijkstra.h"

#include <list>

using namespace std;

Dijkstra::Dijkstra()
{
	for (int x = 0; x < NUM_TILES_X; x++)
		for (int y = 0; y < NUM_TILES_Y; y++)
			isWalkable[x][y] = false;
	
}

Dijkstra::~Dijkstra()
{}

void Dijkstra::SetWalkableMap(const std::vector<iPoint>& walkableTiles)
{
	for (auto tile : walkableTiles)
	{
		isWalkable[tile.x][tile.y] = true;
		basicMap.push_back(tile);
	}
}

bool Dijkstra::FindPath(const iPoint& tileStart, const iPoint & tileEnd, vector<iPoint>& path) const
{
	if (!isWalkable[tileStart.x][tileStart.y] || !isWalkable[tileEnd.x][tileEnd.y])
		return false;

	path.clear();

	DNode grid[NUM_TILES_X][NUM_TILES_Y];

	list<DNode*> noVisited, visited;
	DNode* current = nullptr;
	for (auto tile : basicMap)
	{
		if (tile == tileStart)
			current = &grid[tile.x][tile.y];
		else
			noVisited.push_back(&grid[tile.x][tile.y]);
	}

	while (current || noVisited.size() > 0)
	{
		
	}

	

	return true;
}
