#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "Definitions.h"
#include "Cell.h"
#include "CompareCells.h"
#include "Base.h"
#include "Definitions.h"
#include "Bullet.h"
#include "Grenade.h"

using namespace std;

class State;

class NPC
{
private:

	Base* pBase;
	
	int hp; // health points
	int teamColor;
	int numOfBullets, numOfGrenades;

	int row, col; // NPC's position
	int targetRow, targetCol; // target's position

	bool isAttacking, isMoving;
	bool atBase, goingToBase, goingToEnemy;
	bool hasPath, fire;
	bool isDead;
	State* pCurrentState;
	priority_queue<Cell*, vector<Cell*>, CmpCellF> pqAStar;
	stack<int> path;
	Bullet* bullet;
	Grenade* grenade;
	//State* pInterruptedState;

public:
	NPC(Base* pB, int x, int y, int teamColor);
	~NPC();


	// getters
	Base* getBase() { return pBase; }
	int getHP() { return hp; }
	int GetRow() { return row; }
	int GetCol() { return col; }
	int getTeamColor() { return teamColor; }
	int getTargetX() { return targetRow; }
	int getTargetY() { return targetCol; }
	void GotBullet(int num);
	State* getCurrentState() { return pCurrentState; }
	Bullet* getBullet() { return bullet; }
	void StopBulletAfterHit() { bullet = nullptr; }
	//State* getInterruptedState() { return pInterruptedState; }


	// setters
	void setIsAttacking(bool value) { isAttacking = value; }
	void setIsMoving(bool value) { isMoving = value; }
	void setAtBase(bool value) { atBase = value; }
	void setGoingToBase(bool value) { goingToBase = value; }
	void setGoingToEnemy(bool value) { goingToEnemy = value; }
	void setCurrentState(State* ps) { pCurrentState = ps; }
	//void setInterruptedState(State* ps) { pInterruptedState = ps; }
	void setNumOfBullets(int num) { numOfBullets = num; }
	void setNumOfGrenades(int num) { numOfGrenades = num; }
	void setTarget(double targetX, double targetY);

	void CalcMove(int maze[MAP_SIZE][MAP_SIZE], int targetRow, int targetCol, int targetType);
	void Move(int maze[MAP_SIZE][MAP_SIZE]);
	int ManhattanDistance(int row, int col, int targetRow, int targetCol);
	void FoundPath(Cell* pCurr, int maze[MAP_SIZE][MAP_SIZE]);
	void CleanMaze(int maze[MAP_SIZE][MAP_SIZE]);
	void NewTarget(int maze[MAP_SIZE][MAP_SIZE]);
	void DoSomething(int maze[MAP_SIZE][MAP_SIZE]); // kind of MAIN function
	void DrawMe();
	double CalculateDistanceFromTarget();
	bool hasArrivedToBase();
	bool SearchInRoom(int maze[MAP_SIZE][MAP_SIZE]);
};

