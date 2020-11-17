#include "Grid.h"


Node::Node()
{
	row = -1;
	col = -1;
	isWalkable = false;
}

Grid::Grid()
{
	Grid(default_size);
}

Grid::Grid(int size)
{
	Size = size;
	Root = new Node * [Size];
	for (int i = 0; i < Size; i++)
	{
		Root[i] = new Node[Size];
		for (int j = 0; j < Size; j++)
		{
			Root[i][j].row = i;
			Root[i][j].col = j;
		}
	}
}

Grid::~Grid()
{
	for (int i = 0; i < Size; i++)	delete[] Root[i];
	delete[] Root;
}

int Grid::GetSize()
{
	return Size;
}

Node* Grid::GetNode(int row, int col)
{
	return &Root[row][col];
}

char Grid::GetAppropriateNodeOutput(Node* node)
{
	return node->isWalkable ? OUTPUT_IF_WALKABLE : OUTPUT_IF_NOT_WALKABLE;
}

void Grid::Print()
{
	for (int i = 0; i < Size; ++i) {
		for (int j = 0; j < Size; ++j)
			cout << GetAppropriateNodeOutput(GetNode(i, j)) << " ";
		cout << endl;
	}
}

vector<Node*> Grid::FindPath(Node* start, Node* destination)
{
	Astar pathFinder;
	return	pathFinder.FindPath(start, destination, Root, Size);
}


vector<Node*> Astar::FindPath(Node* start, Node* destination, Node** root, int size)
{
	Size = size;
	vector<PathNode*> neighbours;
	map<PathNode*, float> openList;
	vector<Node*> Path;
	bool** closedList = new bool* [Size], foundDestination = false;
	PathNode** pathMap = new PathNode * [Size], * curNode, * curBestNode;
	float newG, newH, newf;

	for (int i = 0; i < Size; i++)
	{
		closedList[i] = new bool[Size];
		pathMap[i] = new PathNode[Size];
		for (int j = 0; j < Size; j++) {
			closedList[i][j] = false;
			pathMap[i][j] = PathNode(i, j, nullptr, (&root[i][j])->isWalkable);
		}
	}

	curBestNode = &pathMap[start->row][start->col];
	openList.insert({ curBestNode, curBestNode->fScore() });

	while (openList.size() > 0)
	{
		auto openList_IT = *min_element(openList.begin(), openList.end(),
			[](const auto& l, const auto& r) { return l.second < r.second; });

		curBestNode = openList_IT.first;
		closedList[curBestNode->row][curBestNode->col] = true;
		openList.erase(curBestNode);


		neighbours = GetNeighbours(curBestNode, pathMap);

		for (auto n_IT = neighbours.begin(); n_IT != neighbours.end();)
		{
			if (IsDestination(n_IT[0], destination))
			{
				foundDestination = true;
				break;
			}
			if (!IsWalkable(n_IT[0], pathMap))
				n_IT = neighbours.erase(n_IT);
			else
				n_IT++;
		}

		if (foundDestination)
			break;

		for (int i = 0; i < neighbours.size(); i++)
		{
			curNode = neighbours[i];

			if (closedList[curNode->row][curNode->col] == true)
				continue;

			newG = ++curBestNode->gScore;
			newH = calculateH(curNode->row, curNode->col, destination->row, destination->col);
			newf = newG + newH;

			curNode->hScore = newH;
			curNode->gScore = newG;
			curNode->Parent = curBestNode;

			auto it = openList.find(curNode);
			if (it == openList.end())
				openList.insert({ curNode,newf });
			else if (newf < it->second)
				it->second = newf;

		}
	}

	if (foundDestination)
	{
		Path.insert(Path.begin(), &root[curBestNode->row][curBestNode->col]);
		while (true)
		{
			if (curBestNode->Parent->row == start->row && curBestNode->Parent->col == start->col)
				break;
			curBestNode = curBestNode->Parent;
			Path.insert(Path.begin(), &root[curBestNode->row][curBestNode->col]);
		}
	}

	for (int i = 0; i < Size; i++)
	{
		delete[] pathMap[i];
		delete[] closedList[i];
	}
	delete[] pathMap;
	delete[] closedList;

	return Path;
}

float Astar::calculateH(int row, int col, int destRow, int destCol)
{
	return (abs(destRow - row) + abs(destCol - col));
}

bool Astar::IsValid(int row, int col)
{
	return row >= 0 && row <= Size - 1 && col >= 0 && col <= Size - 1;
}

bool Astar::IsWalkable(PathNode* node, PathNode** root)
{
	return IsValid(node->row, node->col) && (&root[node->row][node->col])->isWalkable;
}

bool Astar::IsDestination(PathNode* node, Node* destination)
{
	return node->row == destination->row && node->col == destination->col;
}

vector<PathNode*> Astar::GetNeighbours(PathNode* node, PathNode** root)
{
	vector<PathNode*> neighbours;
	if (IsValid(node->row - 1, node->col))neighbours.push_back(&root[node->row - 1][node->col]);
	if (IsValid(node->row + 1, node->col))neighbours.push_back(&root[node->row + 1][node->col]);
	if (IsValid(node->row, node->col + 1))neighbours.push_back(&root[node->row][node->col + 1]);
	if (IsValid(node->row, node->col - 1))neighbours.push_back(&root[node->row][node->col - 1]);
	return neighbours;
}


PathNode::PathNode()
{
	PathNode(-1, -1, nullptr, false);
}

PathNode::PathNode(int row, int col, PathNode* parent, bool canWalk)
{
	gScore = -1;
	hScore = -1;
	this->row = row;
	this->col = col;
	this->Parent = parent;
	this->isWalkable = canWalk;
}

float PathNode::fScore()
{
	return hScore == -1 ? -1 : gScore + hScore;
}
