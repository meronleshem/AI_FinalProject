#include "GoToBase.h"
#include "StayAtBase.h"

GoToBase::GoToBase()
{
}

GoToBase::~GoToBase()
{
}

void GoToBase::Transform(NPC* pNpc)
{
	OnExit(pNpc);
	pNpc->setCurrentState(new StayAtBase());
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void GoToBase::OnEnter(NPC* pNpc)
{
	pNpc->setGoingToBase(true);
	pNpc->setIsMoving(true);
}

void GoToBase::OnExit(NPC* pNpc)
{
	pNpc->setGoingToBase(false);
	pNpc->setIsMoving(false);
}