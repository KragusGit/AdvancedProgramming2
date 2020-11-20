#include "Maze.h"
#include<fstream>
#include "Main.h"
#include "Player.h"
#include<chrono>
#include<thread>
#include <string>
#include <filesystem>

using namespace std;
namespace fileSys = std::filesystem;

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

Maze* maze;
CustomNodeInfo** CustomInfo;
vector<vector<int>>  Path;
Player* players;
string fileString;
int numberOfExits;
int maze_size;
fstream fs;

const string FOLDER_NAME = "SavedMazes";
const string MAIN_MENU_TITLE = "Please Choose one from these options:";
const string MAIN_MENU_OPTION1 = "1. Generate a new Maze.";
const string MAIN_MENU_EXIT_HINT = "2. EXIT. ";
const string MAIN_MENU_INPUT_HINT = "To select an option, type respective numbers : ";
const string MAZE_GENERATION_TITLE = "Please answer following questions : ";
const string MAZE_GENERATION_SIZE_INPUT_HINT = "Enter size of maze(Max - 50, MIN - 5) : ";
const string MAZE_GENERATION_EXIT_INPUT_HINT = "Enter number of exits to create(Min - 2) : ";
const string SAVE_FILE_HINT = "Do You Want To Save This Maze ? (y/n) : ";
const string GENERIC_ERROR_MESSAGE = "Wrong Input! Please try again. ";
const int MAX_MAZE_SIZE = 50;

void PrintMaze(Maze* maze)
{
	char charToPrint;
	for (int i = 0; i < maze->GetSize(); ++i) {
		for (int j = 0; j < maze->GetSize(); ++j) {

			if (((i == maze->GetSize() / 2 && j == maze->GetSize() / 2)))
				charToPrint = 'F';
			else if (CustomInfo[i][j].isOccupied)
				charToPrint = ('0' + (CustomInfo[i][j].occupiedByPlayerIndex));
			else
				charToPrint = maze->GetAppropriateNodeOutput(maze->GetNode(i, j));

			cout << charToPrint << " ";
			fileString += charToPrint;
			fileString += " ";
		}
		cout << endl;
		fileString += '\r\n';
	}
	fileString += '\r\n';
}

void InitializeCuztomInfo(int size)
{
	CustomInfo = new CustomNodeInfo * [size];
	for (int i = 0; i < size; i++)
		CustomInfo[i] = new CustomNodeInfo[size];
}

void GenerateMaze()
{
	maze = new Maze(maze_size, numberOfExits);
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
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
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
			players[i].AddPathPointInBack(Path[j][0], Path[j][1]);

		players[i].AddPathPointInFront(maze->Exits[i]->row, maze->Exits[i]->col);
		players[i].AddPathPointInBack(maze->GetSize() / 2, maze->GetSize() / 2);
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
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			cout << GENERIC_ERROR_MESSAGE << endl;
		}
		else
			break;
	}

	switch (choice)
	{
	case 1: ShowMazeGenerationMenu(); break;
	case 2: EXIT(); break;
	}

}

void main()
{
	srand(time(NULL));
	while (true)
	{
		system("CLS");
		fileString.clear();
		ShowMainMenu();
		ShowSaveOption();
		system("pause");
	}
}

void ShowSaveOption()
{
	char input;
	while (true)
	{
		cout << SAVE_FILE_HINT;
		cin >> input;
		cout << endl;
		if (input == 'y' || input == 'Y' || input == 'n' || input == 'N')
			break;
		else
			cout << GENERIC_ERROR_MESSAGE << endl;
	}
	switch (input)
	{
	case 'Y':
	case 'y':WriteToFile(); break;
	}
}

void WriteToFile()
{
	string fileName;
	int fileNumber = 1;
	if (!fileSys::exists(FOLDER_NAME) || !fileSys::is_directory(FOLDER_NAME))
		fileSys::create_directory(FOLDER_NAME);

	while (true)
	{
		fileName = "Maze_" + std::to_string(fileNumber) + ".txt";
		if (fileSys::exists(FOLDER_NAME + "\\" + fileName))
			fileNumber++;
		else
			break;
	}

	fs.open(FOLDER_NAME + "\\" + fileName, fstream::app);
	if (fs.is_open())
	{
		fs << fileString;
		fs.close();
		cout << "Maze saved in " << FOLDER_NAME + "/" + fileName << endl;
	}
	else
		cout << "Maze could not be saved!" << endl;
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
		if (AllPlayerReached()) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}
}