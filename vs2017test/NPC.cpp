#include "NPC.h"
#include "glut.h"
#include "GetCloserToEnemy.h"
#include "GetHpAndAmmo.h"

NPC::NPC(Base* pB, int x, int y, int teamColor, NPC* oppTeam[]) :
	pBase(pB), row(x), col(y), teamColor(teamColor)
{
	for (int i = 0; i < NUM_OF_PLAYERS; i++)
		this->oppTeam[i] = oppTeam[i];
	targetInd = rand() % 3;
	this->hp = MAX_HP;
	this->numOfBullets = 500;
	this->numOfGrenades = 3;

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

void NPC::GotBullet(int num)
{
	if (isDead)
		return;

	hp -= rand() % 30 + 20;
	string team = "Red";
	if (teamColor == BLUE_TEAM_COLOR)
		team = "Blue";
	if (hp <= 0)
	{
		isDead = true;
		cout << "-----------" << team << num << " Dead-----------" << endl;
	}
	else
		cout << team << num << " " << hp << " hp" << endl;
	
}

void NPC::setTarget(double targetX, double targetY)
{

}

void NPC::setAsCarrier(vector<HPpos>& allHp, vector<AmmoPos>& allAmmo)
{
	isCarrier = true;
	pCurrentState = new GetHpAndAmmo();
	isGetHPandAmmo = true;
	SearchForHPandAmmo(allHp, allAmmo);
}

void NPC::DoSomething(int maze[MAP_SIZE][MAP_SIZE], vector<HPpos>& allHp, vector<AmmoPos>& allAmmo)
{
	if (isDead)
	{
		maze[row][col] = SPACE;
		return;
	}

	if(isCarrier)
	{
		if (isGetHPandAmmo)
		{
			SearchForHPandAmmo(allHp, allAmmo);
		}
		if (isMoving)
		{
			// move npc to next cell by ai path
			if (hasPath)
			{
				Sleep(20);
				Move(maze, allHp, allAmmo);
			}
			else
				CalcMove(maze, targetRow, targetCol, SPACE);
		}
		return;
	}

	if (atBase) 
	{
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
	else 
	{
		if (isMoving)
		{
			// move npc to next cell by ai path
			if (hasPath)
			{
				Sleep(20);
				Move(maze, allHp, allAmmo);
			}
			else
				CalcMove(maze, targetRow, targetCol, SPACE);

			if (numOfBullets > 0 || numOfGrenades > 0) 
			{
				// has enough ammo
				if (CalculateDistanceFromTarget() <= 7.5)
				{
					// close enough to enemy & has ammo -> attack enemy
					pCurrentState->Transform(this);
				}

			}
			return;
		}

		if (isAttacking)
		{
			// shoots more bullets than throws grenades
			if (rand() % 100 < 10) {
				// throw grenade
				if (numOfGrenades > 0)
				{
					grenade = new Grenade(row, col);
					grenade->Explode();
					numOfGrenades--;
				}
			}
			else {
				// shoot a bullet
				if (numOfBullets > 0)
				{
					//calc angel between shooter to target
					double angle = atan2(targetCol - col, targetRow - row) * 180 / 3.14;
					bullet = new Bullet(row, col, angle);
					bullet->Fire();
					//numOfBullets--;
					
				}
			}
				//Check if the target is far
			if (CalculateDistanceFromTarget() > 7.5)
			{
				//Keep searching
				pCurrentState->Transform(this);
			}
			
			return;
		}
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
	int minRange = INT_MAX;
	for (int i = 0; i < NUM_OF_PLAYERS; i++)
	{
		if (oppTeam[i] == nullptr)
			continue;

		if (oppTeam[i]->GetIsDead())
			continue;

		int trow = oppTeam[i]->GetRow();
		int tcol = oppTeam[i]->GetCol();
		int currRange = sqrt(pow(row - trow, 2) + pow(col - tcol, 2));
		if (currRange < minRange)
			minRange = currRange;
	}

	return minRange;
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
				if (ManhattanDistance(row, col, i, j) < 50)
				{
					return true;
					fire = true;
				}
			}
		}
	}

	fire = false;
	return false;
}

void NPC::SearchForTeammate()
{

}

void NPC::SearchForHPandAmmo(vector<HPpos>& allHp, vector<AmmoPos>& allAmmo)
{
	if (searchHp)
	{
		if (!allHp[targetInd].isTaken)
		{
			targetRow = allHp[targetInd].col;
			targetCol = allHp[targetInd].row;
		}
		return;
	}
	else
	{
		if (!allAmmo[targetInd].isTaken)
		{
			targetRow = allAmmo[targetInd].col;
			targetCol = allAmmo[targetInd].row;
		}
		return;
	}
	int choice = rand() % 2;
	if (choice == 0)
	{
		searchHp = true;
		int i = rand() % allHp.size();
		
		if (!allHp[i].isTaken)
		{
			targetRow = allHp[i].col;
			targetCol = allHp[i].row;
			targetInd = i;
		}
		
	}
	else
	{
		searchHp = false;
		int i = rand() % allAmmo.size();
		if (!allAmmo[i].isTaken)
		{
			targetRow = allAmmo[i].col;
			targetCol = allAmmo[i].row;
			targetInd = i;
		}
	}
}

void NPC::CalcMove(int maze[MAP_SIZE][MAP_SIZE], int targetRow, int targetCol, int targetType)
{
	while (!hasPath)
	{
		if (pqAStar.empty() || pqAStar.size() > 5000) // nothing to do
		{
			NewTarget(maze, true);
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
			if (nextCell == WALL || nextCell == GRAY)
				continue;

			if (nextRow == targetRow && nextCol == targetCol)
			{
				Cell* neighbor = new Cell(nextRow, nextCol, pCurrent, tmpG, targetRow, targetCol);
				pqAStar.push(neighbor);
				FoundPath(neighbor, maze);
				break;
			}

			tmpH = ManhattanDistance(nextRow, nextCol, targetRow, targetCol);
			tmpF = tmpH + tmpG;
			Cell* neighbor = new Cell(nextRow, nextCol, pCurrent, tmpG, targetRow, targetCol);
			pqAStar.push(neighbor);
		}
	}
}
void NPC::Move(int maze[MAP_SIZE][MAP_SIZE], vector<HPpos>& allHp, vector<AmmoPos>& allAmmo)
{
	if (path.empty())
	{
		hasPath = false;
		NewTarget(maze, false);
		return;
	}
	maze[this->row][this->col] = SPACE;

	this->col = path.top();
	path.pop();
	this->row = path.top();
	path.pop();

	if (maze[col][row] == HP)
	{
		maze[col][row] = SPACE;

		cout << "HP taken" << endl;
		hp += 30;
		if (hp > 100)
			hp = 100;
		cout << hp << endl;
		for (int i = 0; i < allHp.size(); i++)
		{
			int hpRow = allHp[i].col;
			int hpCol = allHp[i].row;

			if (row == hpRow && col == hpCol && !allHp[i].isTaken)
			{
				allHp[i].isTaken = true;
				return;
			}
		}
	}
	else if (maze[col][row] == AMMO)
	{
		maze[col][row] = SPACE;

		cout << "Ammo taken" << endl;
		numOfBullets += 50;
		cout << numOfBullets << endl;
		for (int i = 0; i < allAmmo.size(); i++)
		{
			int ammoRow = allAmmo[i].col;
			int ammoCol = allAmmo[i].row;
			
			if (row == ammoRow && col == ammoCol && !allAmmo[i].isTaken)
			{
				allAmmo[i].isTaken = true;
				return;
			}
		}
	}

	//maze[this->row][this->col] = teamColor;
}

int NPC::ManhattanDistance(int row, int col, int targetRow, int targetCol)
{
	return abs(row - targetRow) + abs(col - targetCol);
}

void NPC::FoundPath(Cell* pCurr, int maze[MAP_SIZE][MAP_SIZE])
{
	while (pCurr != nullptr)
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

void NPC::NewTarget(int maze[MAP_SIZE][MAP_SIZE], bool random)
{
	CleanMaze(maze);

	if (isCarrier)
	{
		pqAStar = priority_queue<Cell*, vector<Cell*>, CmpCellF>();
		pqAStar.push(new Cell(row, col, nullptr));
		return;
	}

	if (oppTeam[targetInd] != nullptr && oppTeam[targetInd]->isDead)
	{
		while(oppTeam[targetInd] != nullptr && oppTeam[targetInd]->isDead)
			targetInd = rand() % 3;
	}
	if (oppTeam[targetInd] != nullptr && !random)
	{
		targetRow = oppTeam[targetInd]->GetRow();
		targetCol = oppTeam[targetInd]->GetCol();
	}

	else
	{
		targetRow = rand() % MAP_SIZE;
		targetCol = rand() % MAP_SIZE;
		while (maze[targetRow][targetCol] != SPACE)
		{
			targetRow = rand() % MAP_SIZE;
			targetCol = rand() % MAP_SIZE;
		}
	}

	pqAStar = priority_queue<Cell*, vector<Cell*>, CmpCellF>();
	pqAStar.push(new Cell(row, col, nullptr));

}