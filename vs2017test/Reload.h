#pragma once
#include "State.h"
class Reload :
    public State
{
public:
	Reload();
	~Reload();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

