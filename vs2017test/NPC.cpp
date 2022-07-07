#include "NPC.h"
#include "glut.h"
#include "GetCloserToEnemy.h"

NPC::NPC(Base* pB, int x, int y, int teamColor) :
	pBase(pB), row(x), col(y), teamColor(teamColor)
{
	this->hp = MAX_HP;
	this->numOfBullets = rand() % MAX_NUM_OF_BULLETS;
	this->numOfGrenades = rand() % MAX_NUM_OF_GRENADES;

	this->atBase = false;
	this->isAttacking = false;
	this->isMoving = false;
	this->goingToBase = false;
	this->goingToEnemy = false;

	// set target as closest enemy
	targetRow = 0;
	targetCol = 0;

	pCurrentState = new GetCloserToEnemy();
	pCurrentState->OnEnter(this);
}

NPC::~NPC()
{
}

void NPC::setTarget(double targetX, double targetY)
{

}

void NPC::DoSomething(int maze[MAP_SIZE][MAP_SIZE])
{
	
	if (isMoving) {
		// move npc to next cell by ai path
		if (hasPath)
		{
			Sleep(50);
			Move(maze);
		}
		else
			CalcMove(maze, targetRow, targetCol, SPACE);

	//	SearchInRoom(maze);
		
	}

	if (isAttacking) {
		SearchInRoom(maze);
		// shoots more bullets than throws grenades
		if (rand() % 4 == 0) {
			// throw grenade
		}
		else {
			if (fire)
			{
				if (bullet == nullptr || !bullet->getIsMoving())
				{
					bullet = new Bullet(row, col, (rand() % 360) * 3.14 / 180);
					bullet->Fire();

				}
				if (bullet->getIsMoving())
					bullet->Move(maze);
			}
			// shoot a bullet
		}
	}

	if (atBase) {
		if (hp < MAX_HP) {
			// recover
			if (getBase()->useHealthKit()) {
				hp += 10;
			}
		}
		else if (numOfBullets == 0 || numOfGrenades == 0) {
			// reload
			int bulletsFromBase = 0, grenadesFromBase = 0;
			getBase()->reloadAmmo(&bulletsFromBase, &grenadesFromBase);
			numOfBullets += bulletsFromBase;
			numOfGrenades += grenadesFromBase;
		}
		else {
			// npc should get out from base and get closer to enemy
			pCurrentState->Transform(this);
		}
	}
	else {
		// not at base
		if (((hp < LOW_HP) || (numOfBullets == 0 || numOfGrenades == 0)) && !goingToBase) {
			// has low hp or missing ammo -> should go to base
			pCurrentState->Transform(this);
		}
		else if (numOfBullets > 0 && numOfGrenades > 0) {
			// has enough ammo
			if (CalculateDistanceFromTarget() <= 7.5) {
				// close enough to enemy & has ammo -> attack enemy
				pCurrentState->Transform(this);
			}
			else if (!goingToEnemy) {
				// has ammon but enemy not close enough -> get closer to enemy
				pCurrentState->Transform(this);
			}
		}
		else {
			if (goingToBase && hasArrivedToBase()) {
				// arrived to base
				pCurrentState->Transform(this);
			}
		}
	}

}

void NPC::DrawMe()
{
	switch (teamColor) {

	case RED_TEAM_COLOR:
		glColor3d(1, 0, 0);
		break;

	case BLUE_TEAM_COLOR:
		glColor3d(0, 0, 1);
		break;
	}
	
	glBegin(GL_POLYGON);
	glVertex2d(row, col); // left-bottom corner
	glVertex2d(row, col + 1); // left-top corner
	glVertex2d(row + 1, col + 1); // right-top corner
	glVertex2d(row + 1, col); // right-bottom corner
	glEnd();
}

double NPC::CalculateDistanceFromTarget()
{
	return sqrt(pow(row - targetRow, 2) + pow(col - targetCol, 2));
}

bool NPC::hasArrivedToBase()
{
	if (row == pBase->getBaseX() && col == pBase->getBaseY())
		return true;
	return false;
}

bool NPC::SearchInRoom(int maze[MAP_SIZE][MAP_SIZE])
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (maze[i][j] == WALL || maze[i][j] == SPACE)
				continue;
			if (maze[i][j] != teamColor)
			{
				if (ManhattanDistance(row, col, i, j) < 15)
					fire = true;
			}
		}
	}

	//for (int i = 0; i < MAP_SIZE; i++)
	//{
	//	for (int j = 0; j < MAP_SIZE; j++)
	//	{
	//		cout << maze[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	return false;
}

void NPC::CalcMove(int maze[MAP_SIZE][MAP_SIZE], int targetRow, int targetCol, int targetType)
{
	while (!hasPath)
	{
		if (pqAStar.empty() || pqAStar.size() > 1000) // nothing to do
		{
			NewTarget(maze);
			return;
		}

		maze[this->row][this->col] = teamColor;
		int row, col;

		Cell* pCurrent = pqAStar.top();
		pqAStar.pop();

		row = pCurrent->getRow();
		col = pCurrent->getCol();
		maze[row][col] = GRAY;
		int nextRow, nextCol, tmpH, tmpG, tmpF;
		tmpG = pCurrent->getG() + 1;

		for (int i = 0; i < NUM_OF_NEIGHBORS; i++)
		{
			nextRow = row + ROW_DIRECTIONS[i];
			nextCol = col + COL_DIRECTIONS[i];

			int nextCell = maze[nextRow][nextCol];
			if (nextCell != SPACE)
				continue;

			if (nextRow == targetRow && nextCol == targetCol)
			{
				FoundPath(pCurrent, maze);
				break;
			}

			tmpH = ManhattanDistance(nextRow, nextCol, targetRow, targetCol);
			tmpF = tmpH + tmpG;
			Cell* neighbor = new Cell(nextRow, nextCol, pCurrent, tmpG, targetRow, targetCol);
			pqAStar.push(neighbor);
		}
	}
}
void NPC::Move(int maze[MAP_SIZE][MAP_SIZE])
{
	if (path.empty())
	{
		hasPath = false;
		NewTarget(maze);
		return;
	}
	maze[this->row][this->col] = SPACE;

	this->col = path.top();
	path.pop();
	this->row = path.top();
	path.pop();

	//maze[this->row][this->col] = teamColor;
}

int NPC::ManhattanDistance(int row, int col, int targetRow, int targetCol)
{
	return abs(row - targetRow) + abs(col - targetCol);
}

void NPC::FoundPath(Cell* pCurr, int maze[MAP_SIZE][MAP_SIZE])
{
	while (pCurr->getParent() != nullptr)
	{
		path.push(pCurr->getRow());
		path.push(pCurr->getCol());
		pCurr = pCurr->getParent();
	}

	CleanMaze(maze);
	hasPath = true;
}

void NPC::CleanMaze(int maze[MAP_SIZE][MAP_SIZE])
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (maze[i][j] == GRAY)
			{
				maze[i][j] = SPACE;
			}
		}
	}
}

void NPC::NewTarget(int maze[MAP_SIZE][MAP_SIZE])
{
	CleanMaze(maze);

	targetRow = rand() % MAP_SIZE;
	targetCol = rand() % MAP_SIZE;
	while (maze[targetRow][targetCol] != SPACE)
	{
		targetRow = rand() % MAP_SIZE;
		targetCol = rand() % MAP_SIZE;
	}
	pqAStar = priority_queue<Cell*, vector<Cell*>, CmpCellF>();
	pqAStar.push(new Cell(row, col, nullptr));

}