#include "GoReload.h"
#include "Reload.h"

GoReload::GoReload()
{
}

GoReload::~GoReload()
{
}

void GoReload::Transform(NPC* pNpc)
{
	OnExit(pNpc);
	pNpc->setCurrentState(new Reload());
	pNpc->getCurrentState()->OnEnter(pNpc);
}

void GoReload::OnEnter(NPC* pNpc)
{
	pNpc->setIsMoving(true);

}

void GoReload::OnExit(NPC* pNpc)
{
	pNpc->setIsMoving(false);
}