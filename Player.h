#include<vector>
using namespace std;
struct point {
	int x, y;
	point()
	{
		x = 0;
		y = 0;
	}

	point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

};

class Player
{
public:
	Player();

	vector<point*> path;
	int indexInPath;
	bool hasReached;

	void Move();
	point* NextStep();
	point* CurrentStep();

};

