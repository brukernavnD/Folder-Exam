#pragma once
#include "Component.h"

class KillComponent : public Component
{
public:
	//constructor(s)
	KillComponent();
	KillComponent(Entity* InOwner, float InKillY);

	//the y value at which to kill the parent entity
	float KillY = -10.0f;
};

