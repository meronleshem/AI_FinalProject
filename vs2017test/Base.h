#pragma once
#include <iostream>

const int MAX_HEALTH_KITS = 30;
const int MAX_BULLETS = 60;
const int MAX_GRENADES = 15;


class Base
{
private:
	int color;
	int healthKits;
	int bullets;
	int grenades;
	int x, y; // base's position

public:
	Base(int color, int baseX, int baseY);
	~Base();

	int getColor() { return color; }
	int getHealthKits() { return healthKits; }
	int getBullets() { return bullets; }
	int getGrenades() { return grenades; }
	int getBaseX() { return x; }
	int getBaseY() { return y; }

	void addHealthKits(int kits);
	void addBullets(int bullets);
	void addGrenades(int grenades);

	bool useHealthKit();
	void reloadAmmo(int *numOfBullets, int *numOfGrenades);
};

