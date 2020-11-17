#include "Player.h"
Player::Player()
{
	indexInPath = 0;
	hasReached = false;
}
void Player::Move()
{

	indexInPath++;
	if (indexInPath == path.size() - 1)
		hasReached = true;
}

point* Player::NextStep()
{
	return path[indexInPath + 1];
}

point* Player::CurrentStep()
{
	return path[indexInPath];
}
