#include "Maze.h"
#include <time.h>
#include<fstream>
#include "Main.h"
#include "Player.h"
#include <filesystem>
#include<chrono>
#include<thread>

using namespace std;

struct CustomNodeInfo
{
	bool isOccupied;
	int occupiedByPlayerIndex;
	CustomNodeInfo()
	{
		isOccupied = false;
		occupiedByPlayerIndex = -1;
	}
};

_Maze::Maze* maze;
CustomNodeInfo** CustomInfo;
vector<Node*> Path;
ofstream os;
Player* players;

const string MAIN_MENU_TITLE = "Please Choose one from these options:";
const string MAIN_MENU_OPTION1 = "1. Generate a new Maze.";
const string MAIN_MENU_EXIT_HINT = "3. EXIT. ";
const string MAIN_MENU_INPUT_HINT = "To select an option, type respective numbers : ";
const string MAZE_GENERATION_TITLE = "Please answer following questions : ";
const string MAZE_GENERATION_SIZE_INPUT_HINT = "Enter size of maze(Max - 50, MIN - 5) : ";
const string MAZE_GENERATION_EXIT_INPUT_HINT = "Enter number of exits to create(Min - 2) : ";
const string GENERIC_ERROR_MESSAGE = "Wrong Input! Please try again. ";

const int MAX_MAZE_SIZE = 50;
int numberOfExits;
int maze_size;

void PrintMaze(_Maze::Maze* maze)
{
	for (int i = 0; i < maze->GetSize(); ++i) {
		for (int j = 0; j < maze->GetSize(); ++j)
			cout << (CustomInfo[i][j].isOccupied ? (char)(CustomInfo[i][j].occupiedByPlayerIndex - '0') : ((char)(i == maze->GetSize() / 2 && j == maze->GetSize() / 2)) ? 'F' : ((char)maze->GetAppropriateNodeOutput(maze->GetNode(i, j)))) << " ";
		cout << endl;
	}
}

void InitializeCuztomInfo(int size)
{
	CustomInfo = new CustomNodeInfo * [size];
	for (int i = 0; i < size; i++)
		CustomInfo[i] = new CustomNodeInfo[size];
}


void GenerateMaze()
{
	system("CLS");
	maze = new _Maze::Maze(maze_size, numberOfExits);
	InitializeCuztomInfo(maze->GetSize());
}

void EXIT()
{
	exit(0);
}

void ShowMazeGenerationMenu()
{
	int input;
	cout << MAZE_GENERATION_TITLE << endl;
	while (true)
	{
		cout << MAZE_GENERATION_SIZE_INPUT_HINT;
		cin >> input;
		cout << endl;

		if (cin.fail() || input > MAX_MAZE_SIZE || input < 5) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else {
			maze_size = input;
			break;
		}
	}

	cout << MAZE_GENERATION_EXIT_INPUT_HINT << endl;

	while (true)
	{
		cout << MAZE_GENERATION_EXIT_INPUT_HINT << "Maximum allowed( " << maze_size / 2 << " ) : ";
		cin >> input;
		cout << endl;

		if (cin.fail() || input > maze_size / 2 || input < 0) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else
		{
			numberOfExits = input;
			break;
		}
	}
	GenerateMaze();
	InitializePlayers();
	PrintMaze(maze);
	Simulate();
}
void InitializePlayers()
{
	players = new Player[numberOfExits];
	for (int i = 0; i < numberOfExits; i++)
	{
		CustomInfo[maze->Exits[i]->row][maze->Exits[i]->col].isOccupied = true;
		CustomInfo[maze->Exits[i]->row][maze->Exits[i]->col].occupiedByPlayerIndex = i;
		Path = maze->GetPath(maze->Exits[i], maze->GetNode(maze->GetSize() / 2, maze->GetSize() / 2));
		for (int j = 0; j < Path.size(); j++)
			players[i].path.push_back(new point(Path[j]->row, Path[j]->col));

		players[i].path.insert(players[i].path.begin(), new point(maze->Exits[i]->row, maze->Exits[i]->col));
		players[i].path.insert(players[i].path.end(), new point(maze->GetSize() / 2, maze->GetSize() / 2));
	}
}

void ShowMainMenu()
{
	int choice;

	while (true)
	{
		cout << MAIN_MENU_TITLE << endl;
		cout << MAIN_MENU_OPTION1 << endl;
		cout << MAIN_MENU_EXIT_HINT << endl;
		cout << MAIN_MENU_INPUT_HINT;
		cin >> choice;
		cout << endl;
		if (cin.fail() || choice > 3 || choice < 1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else
			break;
	}

	switch (choice)
	{
	case 1: ShowMazeGenerationMenu(); break;
	case 3: EXIT(); break;
	}

}

void main()
{
	srand(time(NULL));
	ShowMainMenu();
	delete maze;
	system("pause");
}

bool AllPlayerReached()
{
	for (int i = 0; i < numberOfExits; i++)
	{
		if (!players[i].hasReached)
			return false;
	}
	return true;
}

void Simulate()
{//Update Loop
	while (true)
	{

		cout << endl;
		for (int i = 0; i < numberOfExits; i++)
		{
			if (players[i].hasReached)
				continue;
			point* nextStep = players[i].NextStep();
			if (CustomInfo[nextStep->x, nextStep->y]->isOccupied)
				continue;
			else
			{
				(&CustomInfo[players[i].CurrentStep()->x][players[i].CurrentStep()->y])->isOccupied = false;
				players[i].Move();
				(&CustomInfo[players[i].CurrentStep()->x][players[i].CurrentStep()->y])->isOccupied = true;
				(&CustomInfo[players[i].CurrentStep()->x][players[i].CurrentStep()->y])->occupiedByPlayerIndex = i;
			}
		}
		PrintMaze(maze);
		if (AllPlayerReached())
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}
}