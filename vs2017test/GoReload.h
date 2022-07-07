#pragma once
#include "State.h"
class GoReload :
	public State
{
public:
	GoReload();
	~GoReload();
	void Transform(NPC* pNpc);
	void OnEnter(NPC* pNpc);
	void OnExit(NPC* pNpc);
};

