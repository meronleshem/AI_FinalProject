#include "Cell.h"
#include <math.h>



Cell::Cell()
{
}

// gets row, column, parent, g, target row and target column
Cell::Cell(int r, int c, Cell *p, double g, int tr, int tc)
{	
	row = r;
	col = c;
	parent = p;
	this->g = g;
	targetRow = tr;
	targetCol = tc;
	ComputeH();
	f = g + h;
}


Cell::~Cell()
{
}

void Cell::ComputeH()
{
	h = sqrt(pow(row - targetRow,2)+pow(col-targetCol,2));
}

