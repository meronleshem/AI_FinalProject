#include "Base.h"


Base::Base(int color, int baseX, int baseY) :
	color(color), x(baseX), y(baseY)
{
	healthKits = MAX_HEALTH_KITS / 2;
	bullets = MAX_BULLETS / 2;
	grenades = MAX_GRENADES / 2;
}

void Base::addHealthKits(int kits)
{
	healthKits += kits;
	if (healthKits > MAX_HEALTH_KITS)
		healthKits = MAX_HEALTH_KITS;
}

void Base::addBullets(int bulletsToAdd)
{
	bullets += bulletsToAdd;
	if (bullets > MAX_BULLETS)
		bullets = MAX_BULLETS;
}

void Base::addGrenades(int grenadesToAdd)
{
	grenades += grenadesToAdd;
	if (grenades > MAX_GRENADES)
		grenades = MAX_GRENADES;
}

bool Base::useHealthKit()
{
	if (healthKits > 0) {
		healthKits--;
		return true;
	}
		
	return false;
}

/**
NPC reload ammo from base function
input: pointers to number of bullets and grenades you get from base
the function random numbers to reload NPC's ammo
*/
void Base::reloadAmmo(int* numOfBullets, int* numOfGrenades)
{
	int randNum;

	if (bullets > 0) {
		randNum = rand() % MAX_BULLETS;
		
		if (bullets >= randNum) {
			*numOfBullets = randNum;
		}
		else {
			*numOfBullets = bullets;
		}

		bullets -= *numOfBullets;
	}

	if (grenades > 0) {
		randNum = rand() % MAX_GRENADES;

		if (grenades >= randNum) {
			*numOfGrenades = randNum;
		}
		else {
			*numOfGrenades = grenades;
		}

		grenades -= *numOfGrenades;
	}
}
