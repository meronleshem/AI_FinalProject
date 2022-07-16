#pragma once
#include "State.h"
class PassHpAndAmmo :
    public State
{
public:
	PassHpAndAmmo();
	~PassHpAndAmmo();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

