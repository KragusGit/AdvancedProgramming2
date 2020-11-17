#include "Maze.h"

using namespace std;
using namespace _Maze;

Maze::Maze() :Grid(default_size)
{
	numberOfExitsRequired = 5;
	Maze(default_size, numberOfExitsRequired);
}

Maze::~Maze()
{
	for (int i = 0; i < Size; i++)
		delete[] CustomInfo[i];
	delete[] CustomInfo;
}

int Maze::GetSize()
{
	return Size;
}

Node* Maze::GetNode(int row, int col)
{
	return Grid::GetNode(row, col);
}

Maze::Maze(int size, int numberOfExits) :Grid(size)
{

	CustomInfo = new CustomNodeInfo * [Size];
	for (int i = 0; i < Size; i++)
	{
		CustomInfo[i] = new CustomNodeInfo[Size];
	}
	numberOfExitsRequired = numberOfExits;
	GenerateMaze();
}

void Maze::Print()
{
	Grid::Print();
}

char Maze::GetAppropriateNodeOutput(Node* node)
{
	return CustomInfo[node->row][node->col].isCenter ? OUTPUT_IF_CENTER :
		CustomInfo[node->row][node->col].isExit ? OUTPUT_IF_EXIT :
		node->isWalkable ? OUTPUT_IF_WALKABLE : OUTPUT_IF_NOT_WALKABLE;
}

vector<Node*> Maze::GetValidWalls(Node* node)
{
	vector<Node*> validWalls;
	if (node->row == 0 || node->row == Size - 1 || node->col == 0 || node->col == Size - 1)
		return validWalls;

	if (node->row - 2 >= 0 && !GetNode(node->row - 2, node->col)->isWalkable) validWalls.push_back(GetNode(node->row - 2, node->col));
	if (node->row + 2 <= Size - 1 && !GetNode(node->row + 2, node->col)->isWalkable) validWalls.push_back(GetNode(node->row + 2, node->col));
	if (node->col - 2 >= 0 && !GetNode(node->row, node->col - 2)->isWalkable) validWalls.push_back(GetNode(node->row, node->col - 2));
	if (node->col + 2 <= Size - 1 && !GetNode(node->row, node->col + 2)->isWalkable) validWalls.push_back(GetNode(node->row, node->col + 2));
	return validWalls;
}

bool Maze::CanBuildPath(Node* node, int direction)
{
	switch (direction)
	{
	case 0:return	node->row > 1 ? (!GetNode(node->row - 1, node->col)->isWalkable) && (node->row - 2 < 0 || GetNode(node->row - 2, node->col)->isWalkable) : false;
	case 1:return	node->col < Size - 2 ? !GetNode(node->row, node->col + 1)->isWalkable && (node->col + 2 >= Size || GetNode(node->row, node->col + 2)->isWalkable) : false;
	case 2:return	node->row < Size - 2 ? !GetNode(node->row + 1, node->col)->isWalkable && (node->row + 2 >= Size || GetNode(node->row + 2, node->col)->isWalkable) : false;
	case 3:return	node->col > 1 ? !GetNode(node->row, node->col - 1)->isWalkable && (node->col - 2 < 0 || GetNode(node->row, node->col - 2)->isWalkable) : false;
	default:return false;
	}
}

int Maze::GetRandomPossibleDirectionToBuild(Node* node)
{
	vector <int> validDir;
	for (int i = 0; i < 4; i++)
		if (CanBuildPath(node, i))  validDir.push_back(i);
	return validDir.size() > 0 ? validDir[rand() % validDir.size()] : -1;
}

void Maze::GenerateMaze()
{
	vector<Node*> walls, temp; Node* curNode;

	CustomInfo[center][center].isCenter = true;
	GetNode(center, center)->isWalkable = true;
	walls.push_back(GetNode(center + 2, center));
	walls.push_back(GetNode(center, center + 2));
	walls.push_back(GetNode(center - 2, center));
	walls.push_back(GetNode(center, center - 2));

	while (walls.size() != 0)
	{
		int wallIndex = rand() % walls.size();
		curNode = walls[wallIndex];
		walls.erase(walls.begin() + wallIndex);

		if (curNode->isWalkable)
			continue;

		curNode->isWalkable = (curNode->row != 0 && curNode->col != 0 && curNode->row != Size - 1 && curNode->col != Size - 1);

		int buildDir = GetRandomPossibleDirectionToBuild(curNode);

		if (buildDir == -1)
			continue;

		GetNode(curNode->row + (buildDir == 0 ? -1 : buildDir == 2 ? 1 : 0),
			curNode->col + (buildDir == 1 ? 1 : buildDir == 3 ? -1 : 0))->isWalkable = true;

		temp = GetValidWalls(curNode);
		walls.insert(walls.end(), temp.begin(), temp.end());
	}

	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			GetNode(center + i, center + j)->isWalkable = true;

	GenerateExits();
}

vector<Node*> Maze::GetPath(Node* start, Node* destination)
{
	if (start != destination)
		return FindPath(start, destination);
}

bool Maze::HasWalkableNeighbour(Node* node)
{
	return	node->row >= 0 && node->row < Size - 1 && GetNode(node->row + 1, node->col)->isWalkable
		|| node->row > 0 && node->row <= Size - 1 && GetNode(node->row - 1, node->col)->isWalkable
		|| node->col >= 0 && node->col < Size - 1 && GetNode(node->row, node->col + 1)->isWalkable
		|| node->col > 0 && node->col <= Size - 1 && GetNode(node->row, node->col - 1)->isWalkable;
}



void Maze::GenerateExits()
{
	vector<Node*> validExits;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			if (i == 0 || j == 0 || i == Size - 1 || j == Size - 1)
			{
				if (HasWalkableNeighbour(GetNode(i, j)))
					validExits.push_back(GetNode(i, j));
			}
		}
	}

	int randNum;
	for (int i = 0; i < numberOfExitsRequired; i++)
	{
		randNum = rand() % validExits.size();
		CustomInfo[validExits[randNum]->row][validExits[randNum]->col].isExit = true;
		Exits.push_back(validExits[randNum]);
		validExits.erase(validExits.begin() + randNum);
		numberOfExitsPlaced++;
	}
}

CustomNodeInfo::CustomNodeInfo()
{
	isCenter = false;
	isExit = false;
}
