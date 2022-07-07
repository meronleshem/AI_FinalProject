#pragma once
#include "State.h"
class AttackEnemy :
    public State
{
public:
	AttackEnemy();
	~AttackEnemy();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

