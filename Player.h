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
	vector<point*> path;
public:

	Player();
	~Player();
	int indexInPath;
	bool hasReached;

	void Move();
	void AddPathPointInFront(int x, int y);
	void AddPathPointInBack(int x,int y);
	point* NextStep();
	point* CurrentStep();

};

