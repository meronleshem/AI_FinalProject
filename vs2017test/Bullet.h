#pragma once
#include "Definitions.h"
const double SPEED = 0.1;

class Bullet
{

private:
	double x, y;
	double direction_angle;
	bool isMoving;
public:
	Bullet();
	Bullet(double x, double y, double angle);
	void show();
	~Bullet();
	void Fire() { isMoving = true; }
	void Move(int maze[MAP_SIZE][MAP_SIZE]);
	bool getIsMoving() { return isMoving; }
	void SimulateMotion(int maze[MAP_SIZE][MAP_SIZE], double security_map[MAP_SIZE][MAP_SIZE], double damage);
	void SimulateVisibility(int maze[MAP_SIZE][MAP_SIZE], double visibility_map[MAP_SIZE][MAP_SIZE]);

};

