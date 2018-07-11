#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#define NUM_TILES_X 25
#define NUM_TILES_Y 20

#include <vector>
#include <list>
#include "Math2D.h"

struct DNode
{
	bool candidate = false; // Candidate to be visited
	bool visited = false;
	int dst = -1;
	DNode* parent = nullptr;
	iPoint pos;
	int dstToEnd = -1;
};

class Dijkstra
{
public:
	Dijkstra();
	~Dijkstra();

	bool FindPath(const iPoint& tileStart, const iPoint& tileEnd, std::vector<iPoint>& path) const;

	void WalkableTiles();
private:
	void SetWalkableMap(std::vector<iPoint*>* walkableTiles);
	int CalculateDistance(const iPoint& start, const iPoint& end)const;
	void AddNeighbour(DNode* current, int x, int y, DNode* child, std::list<DNode*>& toVisit, const iPoint& tileEnd)const;

private:
	bool isWalkable[NUM_TILES_X][NUM_TILES_Y];
};
extern Dijkstra* g_dijkstra;
#endif