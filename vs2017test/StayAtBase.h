#pragma once
#include "State.h"
class StayAtBase :
    public State
{
public:
	StayAtBase();
	~StayAtBase();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

