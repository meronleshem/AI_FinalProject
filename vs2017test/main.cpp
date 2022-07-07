#include "glut.h"
#include <time.h>
#include <vector>
#include <queue>
#include <iostream>
#include "Room.h"
#include "Cell.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "NPC.h"



using namespace std;

const int NUM_ROOMS = 5;
const int WINDOW_SIZE = 630;
const int ROOM_LENGTH = 2 * MAP_SIZE / 9;
const int ROOMS_POSITIONS[][2] = {
	{2 * MAP_SIZE / 9, 2 * MAP_SIZE / 9},
	{2 * MAP_SIZE / 9, 7 * MAP_SIZE / 9},
	{7 * MAP_SIZE / 9, 2 * MAP_SIZE / 9},
	{7 * MAP_SIZE / 9, 7 * MAP_SIZE / 9},
	{MAP_SIZE / 2, MAP_SIZE / 2}
};
const int NUM_OF_PLAYERS = 3;


int maze[MAP_SIZE][MAP_SIZE] = { 0 };
double security_map[MAP_SIZE][MAP_SIZE] = { 0 };
double visibility_map[MAP_SIZE][MAP_SIZE] = { 0 };

Room rooms[NUM_ROOMS];
bool underConstruction = true;
int r1=0, r2=1; // rooms indices

Base* redBase = nullptr;
Base* blueBase = nullptr;
NPC* redTeam[NUM_OF_PLAYERS] = { nullptr };
NPC* blueTeam[NUM_OF_PLAYERS] = { nullptr };


void AddNPCs();
void AddObstacles();

void CheckNeighbor(
	int row, int col, 
	Cell* pcurrent, priority_queue< Cell, vector<Cell>, CompareCells >& pq, 
	vector< Cell >& grays, vector <Cell>& blacks);

void DrawSquare(int i, int j);
void DigPath(int index1, int index2);
void Display();

void Idle();
void Init();

void RandomPositionInRoom(Room* pr, int* px, int* py);
void RestorePath(Cell* ps);

void SetupRooms();
void ShowMaze();
void ShowNPCs();





int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // double buffering for animation
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(700, 100);
	glutCreateWindow("Final Project");

	glutDisplayFunc(Display); // sets display function as window refresh function
	glutIdleFunc(Idle); // runs all the time when nothing happens

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	Init();

	glutMainLoop(); // starts window queue of events

	return 0;
}


void Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);// color of window background
	
	glOrtho(0, MAP_SIZE, 0, MAP_SIZE ,- 1, 1); 

	srand((unsigned int) time(0));

	SetupRooms();

	AddObstacles();

	AddNPCs();
}

void AddNPCs()
{
	int randX, randY;
	int redBaseX = rooms[1].getCenterX() - 10;
	int redBaseY = rooms[1].getCenterY() + 10;
	int blueBaseX = rooms[2].getCenterX() + 10;
	int blueBaseY = rooms[2].getCenterY() - 10;

	redBase = new Base(RED_TEAM_COLOR, redBaseX, redBaseY);
	blueBase = new Base(BLUE_TEAM_COLOR, blueBaseX, blueBaseY);

	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		RandomPositionInRoom(&rooms[1], &randX, &randY);
		redTeam[i] = new NPC(redBase, randY, randX, RED_TEAM_COLOR);

		RandomPositionInRoom(&rooms[2], &randX, &randY);
		blueTeam[i] = new NPC(blueBase, randY, randX, BLUE_TEAM_COLOR);
	}

	maze[redBaseX][redBaseY] = SPACE;
	maze[blueBaseX][blueBaseY] = SPACE;
}

void RandomPositionInRoom(Room* pr, int* px, int* py)
{
	int roomArea = pr->getHeight() * pr->getWidth();
	do {
		int randNum = rand() % roomArea;
		*px = randNum / pr->getWidth() + pr->getCenterX();
		*py = randNum % pr->getWidth() + pr->getCenterY();
	} while (maze[*px][*py] == WALL);
}

void AddObstacles()
{

	// adding random walls all over
	int num_obstacles = 50;
	for (int i = 0; i < num_obstacles; i++) {
		int row = rand() % MAP_SIZE;
		int col = rand() % MAP_SIZE;
		int adjacentRow = row, adjacentCol = col;
		if (i % 2 == 0) {
			adjacentRow++;
			if (adjacentRow >= MAP_SIZE)
				adjacentRow -= 2;
		}
		else {
			adjacentCol++;
			if (adjacentCol >= MAP_SIZE)
				adjacentCol -= 2;
		}
		maze[row][col] = WALL;
		maze[adjacentRow][adjacentCol];
	}

	// adding a '+' shape of walls in the middle of each room
	for (int room = 0; room < NUM_ROOMS; room++) {

		int fromX = rooms[room].getCenterX() - ROOM_LENGTH / 4, toX = rooms[room].getCenterX() + ROOM_LENGTH / 4;
		int col = rooms[room].getCenterY();
		for (int i = fromX; i < toX; i++) {
			maze[i][col] = WALL;
		}

		int fromY = rooms[room].getCenterY() - ROOM_LENGTH / 4, toY = rooms[room].getCenterY() + ROOM_LENGTH / 4;
		int row = rooms[room].getCenterX();
		for (int i = fromY; i < toY; i++) {
			maze[row][i] = WALL;
		}
	}

}

void SetupRooms()
{
	int w, h, x, y;
	for (int r = 0; r < NUM_ROOMS; r++)
	{
		w = ROOM_LENGTH;
		h = ROOM_LENGTH;
		x = ROOMS_POSITIONS[r][0];
		y = ROOMS_POSITIONS[r][1];

		rooms[r].setWidth(w);
		rooms[r].setHeight(h);
		rooms[r].setCenterX(x);
		rooms[r].setCenterY(y);
		rooms[r].AddMeToMaze(maze);
	}
}

// if the neighbor cell is white  (including target as wll) then add it to pq and paint it gray
// if it is black then do nothing
// if it is gray (it can be target as wll) check 2 cases:
//         1: if F of the new found neghbor isn't better (>=) then of the previously found cell then do notheing
//         2: if  F of the new found neghbor IS better (<) then we have to update the cell parameters!!!
void CheckNeighbor(
	int row, int col, 
	Cell* pcurrent, 
	priority_queue< Cell, vector<Cell>, CompareCells > &pq, 
	vector< Cell > &grays, vector <Cell> &blacks)
{
	double cost,cheap = 0.1, expensive = 0.4;
	vector <Cell>::iterator itrb;
	vector <Cell>::iterator itrg;


	if (maze[row][col] == SPACE) cost = cheap;
	else cost = expensive;

	// create neighbor cell
	Cell* pn = new Cell(row, col, pcurrent, pcurrent->getG() + cost, pcurrent->getTargetRow(), pcurrent->getTargetCol());
	// check color of theis cell
	itrb = find(blacks.begin(), blacks.end(), *pn);
	itrg = find(grays.begin(), grays.end(), *pn);

	// if it is white
	if (itrb == blacks.end() && itrg == grays.end())
	{
		pq.push(*pn);
		grays.push_back(*pn);
	}
	else  // it must be either gray or black 
		if (itrg != grays.end())
	{
		if (pn->getF() < itrg->getF()) // we have to update it
		{
			grays.erase(itrg);
			grays.push_back(*pn);
			// to update (find) pn in pq we need manually to extract all the cells that stand bfore pn in pq including pn
			// to update it and then to push all of them back to pq
			vector <Cell> tmp;
			while(!pq.empty() && !((*pn)== pq.top()))
			{
				tmp.push_back(pq.top());
				pq.pop();
			}
			if (pq.empty())
				cout << " neighbor wasn't found in pq \n";
			else // *pn is now at top of pq
			{
				pq.pop();
				pq.push(*pn);
				// move back to pq all the cells from tmp
				while (!tmp.empty())
				{
					pq.push(tmp.back());
					tmp.pop_back();
				}
			}
		}
	}

}

void RestorePath(Cell* ps)
{
	while (ps->getParent() != nullptr)
	{
		if (maze[ps->getRow()][ps->getCol()] == WALL)
			maze[ps->getRow()][ps->getCol()] = SPACE;
		ps = ps->getParent();
	}
}

// creates path from rooms[index1] to rooms[index2] using A*
void DigPath(int index1, int index2)
{
	Cell* startingCell = new Cell(rooms[index1].getCenterY(), rooms[index1].getCenterX(),
		nullptr, 0, rooms[index2].getCenterY(), rooms[index2].getCenterX());

	Cell* pcurrent = nullptr;
	int currentRow, currentCol;

	priority_queue <Cell, vector<Cell>, CompareCells> pq;
	vector <Cell> grays;
	vector <Cell> blacks;

	vector <Cell>::iterator itr;


	// add Start Cell to pq and paint it gray
	pq.push(*startingCell);
	grays.push_back(*startingCell);

	// start A* . In our case pq shouldn't be empty because we should find the target before all the 
	// cells are over
	while (!pq.empty())
	{
		// pq is not empty so pick the top Cell
		pcurrent = new Cell(pq.top());
		// check if current is target
		if (pcurrent->getRow() == rooms[index2].getCenterY() && pcurrent->getCol() == rooms[index2].getCenterX())
		{
			RestorePath(pcurrent);
			cout << "The path from room " << index1 << " to room " << index2 << " has been found\n";
			return;
		}
		// remove current from pq and paint it black (remove it from grays)
		pq.pop();
		blacks.push_back(*pcurrent);
		// remove it from grays
		itr = find(grays.begin(), grays.end(), *pcurrent); // Cell must have operator ==
		if (itr == grays.end()) // current cell wasn't found: ERROR
		{
			cout << "Error: current wasn't found in grays for room " << index1 << " to room " << index2 << endl;
			return;
		}
		grays.erase(itr);

		// now check the neighbor cells
		currentRow = pcurrent->getRow();
		currentCol = pcurrent->getCol();
		if (currentRow > 0) // UP
			CheckNeighbor(currentRow - 1, currentCol, pcurrent, pq, grays, blacks);
		if (currentRow < MAP_SIZE-1) // DOWN
			CheckNeighbor(currentRow + 1, currentCol, pcurrent, pq, grays, blacks);
		if (currentCol > 0) // LEFT
			CheckNeighbor(currentRow , currentCol-1, pcurrent, pq, grays, blacks);
		if (currentCol < MAP_SIZE-1) // RIGHT
			CheckNeighbor(currentRow , currentCol+1, pcurrent, pq, grays, blacks);
	} // while
	// We shouldn't reach this point normally
	cout << "Error: PQ is empty for room " << index1 << " to room " << index2 << endl;
}

void ShowNPCs()
{
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		if (redTeam[i] != nullptr) {
			redTeam[i]->DrawMe();
		}
		if (blueTeam[i] != nullptr) {
			blueTeam[i]->DrawMe();
		}
	}
}

void ShowMaze() 
{
	int i, j;

	for (i = 0; i < MAP_SIZE; i++) {

		for (j = 0; j < MAP_SIZE; j++) {

			// set color for cell (i,j)
			switch (maze[i][j]) {

			case WALL:
				glColor3d(0, 0, 0);
				break;
			case GRAY:
				glColor3d(1, 1, 1);
				break;
			case SPACE:
				double d = security_map[i][j];
				double v = visibility_map[i][j];
				glColor3d(1 - d - 0.5 * v, 1 - d, 1 - d - 0.5 * v); // white - security map value - visibility map value
				break;
		
			}
			// now show the cell as plygon (square)
			DrawSquare(i, j);

		}
	}
}

void DrawSquare(int i, int j) 
{
	glBegin(GL_POLYGON);
	glVertex2d(j, i); // left-bottom corner
	glVertex2d(j, i + 1); // left-top corner
	glVertex2d(j + 1, i + 1); // right-top corner
	glVertex2d(j + 1, i); // right-bottom corner
	glEnd();
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	
	ShowMaze();
	ShowNPCs();

	glutSwapBuffers(); // show all
}

void Idle()
{
	maze[0][0] = SPACE;
	if (underConstruction)
	{
		DigPath(r1, r2);
		// prepare for the next A*
		r2++;
		if (r2 >= NUM_ROOMS)
		{
			r1++;
			r2 = r1 + 1;
			if (r1 + 1 >= NUM_ROOMS)
				underConstruction=false;
		}
	}

	if (!underConstruction)
	{
		for (int i = 0; i < NUM_OF_PLAYERS; i++) {
			if (redTeam[i] != nullptr) {
				redTeam[i]->DoSomething(maze);
			}
			if (blueTeam[i] != nullptr) {
				blueTeam[i]->DoSomething(maze);
			}
		}
	}
	//Sleep(0);

		
	glutPostRedisplay(); // indirect call to refresh function (display)
}



