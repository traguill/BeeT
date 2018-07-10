#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#define NUM_TILES_X 16
#define NUM_TILES_Y 16

#include <vector>
#include "Math2D.h"

struct DNode
{
	bool visited = false;
	int dst = -1;
	DNode* parent = nullptr;
	iPoint pos;
};

class Dijkstra
{
	Dijkstra();
	~Dijkstra();

	void SetWalkableMap(const std::vector<iPoint>& walkableTiles);
	bool FindPath(const iPoint& tileStart, const iPoint& tileEnd, std::vector<iPoint>& path) const;

private:
	bool isWalkable[NUM_TILES_X][NUM_TILES_Y];
};

#endif