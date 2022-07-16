#include "GetHpAndAmmo.h"
#include "PassHpAndAmmo.h"
#include "GetCloserToEnemy.h"
#include "AttackEnemy.h"
#include "GoToBase.h"

GetHpAndAmmo::GetHpAndAmmo()
{
}

GetHpAndAmmo::~GetHpAndAmmo()
{
}

void GetHpAndAmmo::Transform(NPC* pNpc)
{
	OnExit(pNpc);

	pNpc->setCurrentState(new PassHpAndAmmo());

	pNpc->getCurrentState()->OnEnter(pNpc);
}

void GetHpAndAmmo::OnEnter(NPC* pNpc)
{
	pNpc->setIsMoving(true);
	pNpc->setGetHPandAmmo(true);

	pNpc->setTarget(pNpc->getTargetX(), pNpc->getTargetY());
}

void GetHpAndAmmo::OnExit(NPC* pNpc)
{
	pNpc->setIsMoving(true);
	pNpc->setGetHPandAmmo(false);
}
