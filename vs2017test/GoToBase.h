#pragma once
#include "State.h"
class GoToBase :
    public State
{
public:
	GoToBase();
	~GoToBase();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

