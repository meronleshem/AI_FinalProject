#pragma once
#include "State.h"
class GetCloserToEnemy :
    public State
{
public:
	GetCloserToEnemy();
	~GetCloserToEnemy();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

