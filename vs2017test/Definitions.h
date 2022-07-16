#pragma once
const int MAP_SIZE = 60;
const int WALL = 0;
const int SPACE = 1;
const int AMMO = 2;
const int HP = 3;

const int GRAY = 30;

const int NUM_OF_NEIGHBORS = 4;
const int ROW_DIRECTIONS[] = { 1, -1, 0 ,0 };
const int COL_DIRECTIONS[] = { 0, 0, 1, -1 };

const int RED_TEAM_COLOR = 50;
const int BLUE_TEAM_COLOR = 60;

const int MAX_HP = 100;
const int LOW_HP = MAX_HP / 4;

const int NUM_OF_PLAYERS = 3;
const int MAX_NUM_OF_BULLETS = 20;
const int MAX_NUM_OF_GRENADES = 5;

struct HPpos
{
	int row;
	int col;
	bool isTaken;
};

struct AmmoPos
{
	int row;
	int col;
	bool isTaken;
};
