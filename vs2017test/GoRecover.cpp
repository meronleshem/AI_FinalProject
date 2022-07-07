#include "GoRecover.h"
#include "Recover.h"

GoRecover::GoRecover()
{
}

GoRecover::~GoRecover()
{
}

void GoRecover::Transform(NPC* pNpc)
{
	OnExit(pNpc);

	pNpc->setCurrentState(new Recover());

	pNpc->getCurrentState()->OnEnter(pNpc);
}

void GoRecover::OnEnter(NPC* pNpc)
{
	pNpc->setIsMoving(true);

	// find team member location with health kits
	pNpc->setTarget(pNpc->getTargetX(), pNpc->getTargetY());
}

void GoRecover::OnExit(NPC* pNpc)
{
	pNpc->setIsMoving(false);
}