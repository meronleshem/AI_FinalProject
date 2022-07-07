#pragma once
class Cell
{
private:
	int row, col;
	int targetRow, targetCol;
	Cell* parent;
	double f, g, h;

public:
	Cell();
	Cell(int r, int c, Cell* p, double g = 0, int tr = 0, int tc = 0);
	~Cell();

	int getRow() { return row; }
	int getCol() { return col; }
	int getTargetRow() { return row; }
	int getTargetCol() { return col; }
	Cell* getParent() { return parent; }
	void ComputeH();
	void ComputeF() { f = g + h; }
	double getF() const { return f; }
	double getG() { return g; }
	bool operator == (const Cell& other) { return row == other.row && col == other.col; }
	void setG(double newG) { g = newG; }
};

struct CmpCellF
{
	bool operator()(const Cell* c1, const Cell* c2) const
	{
		return c1->getF() > c2->getF();
	}
};
