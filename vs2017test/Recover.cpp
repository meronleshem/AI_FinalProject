#include "Recover.h"

Recover::Recover()
{
}

Recover::~Recover()
{
}

void Recover::Transform(NPC* pNpc)
{
	OnExit(pNpc);
	pNpc->setCurrentState(pNpc->getInterruptedState());
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void Recover::OnEnter(NPC* pNpc)
{
	pNpc->setAtBase(true);
}

void Recover::OnExit(NPC* pNpc)
{
	pNpc->setAtBase(false);
}