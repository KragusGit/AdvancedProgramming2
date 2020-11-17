#include "Grid.h"

namespace _Maze
{
	const char OUTPUT_IF_EXIT = 'E';
	const char OUTPUT_IF_CENTER = 'S';

	class CustomNodeInfo
	{
	public:
		bool isCenter, isExit;
		CustomNodeInfo();
	};

	class Maze :private Grid
	{
	private:
		CustomNodeInfo** CustomInfo;
		const int center = Size / 2;
		int numberOfExitsRequired;
		int numberOfExitsPlaced;
		vector<Node*> GetValidWalls(Node* node);
		bool CanBuildPath(Node* node, int direction);
		int GetRandomPossibleDirectionToBuild(Node* node);
		bool HasWalkableNeighbour(Node* node);
		void GenerateExits();


	public:
		vector<Node*> Exits;
		Maze();
		~Maze();
		int	GetSize();
		Node* GetNode(int row, int col);
		Maze(int size, int numberOfExits);
		virtual void Print();
		virtual char GetAppropriateNodeOutput(Node* node);
		void GenerateMaze();
		vector<Node*> GetPath(Node* start, Node* destination);
	};
}