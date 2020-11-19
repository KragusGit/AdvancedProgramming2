#include "Player.h"
Player::Player()
{
	indexInPath = 0;
	hasReached = false;
}
Player::~Player()
{
	path.clear();
}
void Player::Move()
{

	indexInPath++;
	if (indexInPath == path.size() - 1)
		hasReached = true;
}

void Player::AddPathPointInFront(int x, int y)
{
	path.insert(path.begin(), new point(x, y));
}

void Player::AddPathPointInBack(int x, int y)
{
	path.push_back(new point(x, y));
}

point* Player::NextStep()
{
	return path[indexInPath + 1];
}

point* Player::CurrentStep()
{
	return path[indexInPath];
}
