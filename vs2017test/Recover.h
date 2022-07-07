#pragma once
#include "State.h"
class Recover :
    public State
{
public:
	Recover();
	~Recover();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

