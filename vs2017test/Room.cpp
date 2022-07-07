#include "Room.h"



Room::Room()
{
}


Room::~Room()
{
}

void Room::AddMeToMaze(int maze[MAP_SIZE][MAP_SIZE])
{
	int i, j;

	for (i = centerY - height / 2; i <= centerY + height / 2; i++)
		for (j = centerX - width / 2; j <= centerX + width / 2; j++)
			maze[i][j] = SPACE;
}
