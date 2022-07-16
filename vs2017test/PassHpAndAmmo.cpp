#include "GetHpAndAmmo.h"
#include "PassHpAndAmmo.h"
#include "GetCloserToEnemy.h"
#include "AttackEnemy.h"
#include "GoToBase.h"

PassHpAndAmmo::PassHpAndAmmo()
{
}

PassHpAndAmmo::~PassHpAndAmmo()
{
}

void PassHpAndAmmo::Transform(NPC* pNpc)
{
	OnExit(pNpc);

	pNpc->setCurrentState(new GetHpAndAmmo());

	pNpc->getCurrentState()->OnEnter(pNpc);
}

void PassHpAndAmmo::OnEnter(NPC* pNpc)
{
	pNpc->setPassHPandAmmo(true);
	pNpc->setIsMoving(true);

	pNpc->setTarget(pNpc->getTargetX(), pNpc->getTargetY());
}

void PassHpAndAmmo::OnExit(NPC* pNpc)
{
	pNpc->setPassHPandAmmo(false);
	pNpc->setIsMoving(true);
}
