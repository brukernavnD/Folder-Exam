#include "KillSystem.h"
#include "WorldSystem.h"

void KillSystem::CheckKill(std::vector<RainEntity*>& InRainEntities)
{
	//iterate over the entities
	for (RainEntity*& CurrentEntity : InRainEntities)
	{
		//check if the entity is below the kill y value
		if (CurrentEntity->RainCollisionComponent.Position.y < CurrentEntity->RainKillComponent.KillY)
		{
			//remove the entity from the world
			GetWorld()->DestroyObject(CurrentEntity);
		}
	}
}
