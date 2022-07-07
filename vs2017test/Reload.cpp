#include "Reload.h"

Reload::Reload()
{
}

Reload::~Reload()
{
}

void Reload::Transform(NPC* pNpc)
{
	OnExit(pNpc);
	pNpc->setCurrentState(pNpc->getInterruptedState());
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void Reload::OnEnter(NPC* pNpc)
{
	pNpc->setAtBase(true);
}

void Reload::OnExit(NPC* pNpc)
{
	pNpc->setAtBase(false);
}