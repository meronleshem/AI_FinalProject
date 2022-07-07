#include "StayAtBase.h"
#include "GetCloserToEnemy.h"

StayAtBase::StayAtBase()
{
}

StayAtBase::~StayAtBase()
{
}

void StayAtBase::Transform(NPC* pNpc)
{
	OnExit(pNpc);
	pNpc->setCurrentState(new GetCloserToEnemy());
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void StayAtBase::OnEnter(NPC* pNpc)
{
	pNpc->setAtBase(true);
}

void StayAtBase::OnExit(NPC* pNpc)
{
	pNpc->setAtBase(false);
}