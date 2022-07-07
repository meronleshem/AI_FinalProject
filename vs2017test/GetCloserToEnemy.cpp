#include "GetCloserToEnemy.h"
#include "AttackEnemy.h"
#include "GoToBase.h"

GetCloserToEnemy::GetCloserToEnemy()
{
}

GetCloserToEnemy::~GetCloserToEnemy()
{
}

void GetCloserToEnemy::Transform(NPC* pNpc)
{
	OnExit(pNpc);

	if (pNpc->getHP() < LOW_HP) {
		pNpc->setCurrentState(new GoToBase());
	}
	else {
		pNpc->setCurrentState(new AttackEnemy());
	}

	pNpc->getCurrentState()->OnEnter(pNpc);
}

void GetCloserToEnemy::OnEnter(NPC* pNpc)
{
	pNpc->setGoingToEnemy(true);
	pNpc->setIsMoving(true);

	// find closest enemy's position
	// pNpc->getAiUtils()->findClosestEnemy();

	pNpc->setTarget(pNpc->getTargetX(), pNpc->getTargetY());
}

void GetCloserToEnemy::OnExit(NPC* pNpc)
{
	pNpc->setGoingToEnemy(false);
	pNpc->setIsMoving(false);
}
