#pragma once
#include "Bullet.h"

const int NUM_BULLETS = 50;

class Grenade
{

private: 
	double x, y;
	Bullet* bullets[NUM_BULLETS];
	bool isExploded;
public:
	Grenade();
	Grenade(double x, double y);
	~Grenade();
	bool getIsExploded() { return isExploded; }
	void Explode();
	void Exploding(int maze[MAP_SIZE][MAP_SIZE]);
	void show(int teamColor);
	void SimulateExplosion(int maze[MAP_SIZE][MAP_SIZE], double security_map[MAP_SIZE][MAP_SIZE], double damage);
	void SimulateVisibility(int maze[MAP_SIZE][MAP_SIZE], double visibility_map[MAP_SIZE][MAP_SIZE]);
};

