#pragma once
#include "State.h"
class GoRecover :
    public State
{
public:
	GoRecover();
	~GoRecover();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

