#include "StaticEmitterEntity.h"

StaticEmitterEntity::StaticEmitterEntity() : Entity("StaticEmitterEntity")
{
	//set the class id of the entity
	ClassID = StaticEmitterEntityClassID;

	//set the components of the entity
	Components.push_back(&EmitterComp);
}

StaticEmitterEntity::StaticEmitterEntity(const glm::vec3& InPosition, const glm::vec3& InScale) : Entity("StaticEmitterEntity"), EmitterComp(this, InPosition, InScale)
{
	//set the class id of the entity
	ClassID = StaticEmitterEntityClassID;

	//set the components of the entity
	Components.push_back(&EmitterComp);
}
