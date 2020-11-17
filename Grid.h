#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

const int default_size = 20;
const char OUTPUT_IF_WALKABLE = ' ';
const char OUTPUT_IF_NOT_WALKABLE = 'X';


class Node
{
public:
	int row, col;
	bool isWalkable;
	Node();
};

class Grid
{
private:
	Node** Root;

protected:
	int Size = 0;

public:
	Grid();
	Grid(int size);
	~Grid();
	int GetSize();
	Node* GetNode(int row, int col);
	virtual char GetAppropriateNodeOutput(Node* node);
	virtual void Print();
	vector<Node*> FindPath(Node* start, Node* destination);
};

class PathNode
{
public:
	PathNode();
	PathNode(int row, int col, PathNode* parent, bool canWalk);
	int row, col;
	bool isWalkable;
	float gScore, hScore;
	PathNode* Parent;

	float fScore();
};

class Astar
{
private:
	int Size;
	float calculateH(int row, int col, int destRow, int destCol);
	bool IsValid(int row, int col);
	bool IsWalkable(PathNode* node, PathNode** root);

	bool IsDestination(PathNode* node, Node* Destination);

	vector<PathNode*> GetNeighbours(PathNode* node, PathNode** root);

public:
	
	vector<Node*> FindPath(Node* start, Node* destination, Node** root, int size);
};

