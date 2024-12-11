#pragma once
#include "../Core/Helpers.h"

class Entity;

class VelocitySystem
{
public:

	//function to update the velocity of a single entity
	static void UpdateVelocity(Entity* InEntity, float DeltaTime);

	//function to update the velocity of a list of entities
	static void UpdateVelocities(const std::vector<Entity*>& InEntities, float DeltaTime);
};

