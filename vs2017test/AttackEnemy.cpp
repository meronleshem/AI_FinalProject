#include "AttackEnemy.h"
#include "GoToBase.h"
#include "GetCloserToEnemy.h"

AttackEnemy::AttackEnemy()
{
}

AttackEnemy::~AttackEnemy()
{
}

void AttackEnemy::Transform(NPC* pNpc)
{
	OnExit(pNpc);

	if (pNpc->getHP() < LOW_HP) {
		pNpc->setCurrentState(new GoToBase());
	}
	else {
		pNpc->setCurrentState(new GetCloserToEnemy());
	}
	
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void AttackEnemy::OnEnter(NPC* pNpc)
{
	pNpc->setIsAttacking(true);
}

void AttackEnemy::OnExit(NPC* pNpc)
{
	pNpc->setIsAttacking(false);
}