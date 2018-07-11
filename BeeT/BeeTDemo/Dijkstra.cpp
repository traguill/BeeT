#include "Dijkstra.h"

using namespace std;

Dijkstra::Dijkstra()
{
	for (int x = 0; x < NUM_TILES_X; x++)
		for (int y = 0; y < NUM_TILES_Y; y++)
			isWalkable[x][y] = false;
	
}

Dijkstra::~Dijkstra()
{}


void Dijkstra::SetWalkableMap(std::vector<iPoint*>* walkableTiles)
{
	for (auto tile : *walkableTiles)
	{
		isWalkable[tile->x][tile->y] = true;
	}
}

bool CompareNodes(const DNode* a, const DNode* b)
{
	return a->dstToEnd < b->dstToEnd;
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

		AddNeighbour(current, current->pos.x - 1, current->pos.y, &grid[current->pos.x - 1][current->pos.y], toVisit, tileEnd);
		AddNeighbour(current, current->pos.x + 1, current->pos.y, &grid[current->pos.x + 1][current->pos.y], toVisit, tileEnd);
		AddNeighbour(current, current->pos.x, current->pos.y + 1, &grid[current->pos.x][current->pos.y + 1], toVisit, tileEnd);
		AddNeighbour(current, current->pos.x, current->pos.y - 1, &grid[current->pos.x][current->pos.y - 1], toVisit, tileEnd);
		
		toVisit.sort(CompareNodes);
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

int Dijkstra::CalculateDistance(const iPoint & start, const iPoint & end) const
{
	iPoint v = end - start;
	return sqrt(v.x * v.x + v.y * v.y);
}

void Dijkstra::AddNeighbour(DNode* current, int x, int y, DNode*child, list<DNode*>& toVisit, const iPoint& tileEnd)const
{
	if (isWalkable[x][y] && child->visited == false)
	{
		if (child->candidate == false)
		{
			child->parent = current;
			child->pos = iPoint(x, y);
			child->dstToEnd = CalculateDistance(child->pos, tileEnd);
			child->candidate = true;
			toVisit.push_back(child);
		}
	}
}



void Dijkstra::WalkableTiles()
{
	vector<iPoint*> t;
	for (int i = 0; i <= 20; i++)
	{
		t.push_back(new iPoint(i, 4));
		t.push_back(new iPoint(i, 5));
	}

	for (int i = 6; i <= 14; i++)
	{
		t.push_back(new iPoint(18, i));
		t.push_back(new iPoint(19, i));
		t.push_back(new iPoint(20, i));
	}

	for (int i = 5; i <= 17; i++)
	{
		t.push_back(new iPoint(i, 8));
	}

	for (int x = 5; x <= 11; x++)
		for (int y = 9; y <= 14; y++)
			t.push_back(new iPoint(x, y));

	for (int x = 12; x <= 16; x++)
		for (int y = 11; y <= 14; y++)
			t.push_back(new iPoint(x, y));

	for (int i = 15; i <= 19; i++)
	{
		t.push_back(new iPoint(6, i));
		t.push_back(new iPoint(7, i));
	}
	SetWalkableMap(&t);
	for (auto tile : t)
		delete tile;
	t.clear();
}