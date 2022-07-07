#pragma once
#include "NPC.h"

class State
{
public:
	virtual void Transform(NPC* pNpc) = 0; 
	virtual void OnEnter(NPC* pNpc) = 0;
	virtual void OnExit(NPC* pNpc) = 0;
};

