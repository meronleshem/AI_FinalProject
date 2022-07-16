#pragma once
#include "State.h"
class GetHpAndAmmo :
    public State
{
public:
	GetHpAndAmmo();
	~GetHpAndAmmo();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

