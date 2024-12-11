#include "VelocitySystem.h"
#include "../Entities/Entity.h"
#include "../Components/VelocityComponent.h"
#include "../Components/TransformComponent.h"

void VelocitySystem::UpdateVelocity(Entity* InEntity, const float DeltaTime)
{
	//get the velocity component of the entity
	VelocityComponent* VelocityComp = InEntity->GetComponent<VelocityComponent>();

	//check if the entity does not have a velocity component
	if (!VelocityComp)
	{
		//return if the entity does not have a velocity component
		return;
	}

	//get the transform component of the entity
	TransformComponent* TransformComp = InEntity->GetComponent<TransformComponent>();

	//check if the entity does not have a transform component
	if (!TransformComp)
	{
		//return if the entity does not have a transform component
		return;
	}

	//update the position of the entity
	TransformComp->Position += VelocityComp->Velocity * DeltaTime;

	//update the velocity of the entity
	VelocityComp->Velocity += VelocityComp->Acceleration * DeltaTime;
}

void VelocitySystem::UpdateVelocities(const std::vector<Entity*>& InEntities, const float DeltaTime)
{
	//loop through the entities
	for (Entity* WorldObject : InEntities)
	{
		//get the velocity component of the entity
		VelocityComponent* VelocityComp = WorldObject->GetComponent<VelocityComponent>();

		//check if the entity does not have a velocity component
		if (!VelocityComp)
		{
			//continue to the next object
			continue;
		}

		//get the transform component of the entity
		TransformComponent* TransformComp = WorldObject->GetComponent<TransformComponent>();

		//check if the entity does not have a transform component
		if (!TransformComp)
		{
			//continue to the next object
			continue;
		}

		//update the position of the entity
		TransformComp->Position += VelocityComp->Velocity * DeltaTime;

		//update the velocity of the entity
		VelocityComp->Velocity += VelocityComp->Acceleration * DeltaTime;
	}
}
