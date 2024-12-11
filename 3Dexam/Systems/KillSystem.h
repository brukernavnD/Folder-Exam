#pragma once
#include <vector>

#include "../Entities/RainEntity.h"

class Entity;

class KillSystem
{
public:

	//function to kill entities that are below a certain y value
	static void CheckKill(std::vector<RainEntity*>& InRainEntities);
};

