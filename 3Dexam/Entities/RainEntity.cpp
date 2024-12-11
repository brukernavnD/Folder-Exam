#include "RainEntity.h"

RainEntity::RainEntity() : Entity("Ball")
{
	//set the class id of the ball
	ClassID = RainEntityClassID;
}

RainEntity::RainEntity(const glm::vec3 InPosition, const glm::vec3 InScale, const glm::vec3 InVelocity, const glm::vec3 InAcceleration) : Entity("RainEntity"), RainMeshComponent(this, SphereRenderingID), RainVelocityComponent(this, InVelocity, InAcceleration), RainCollisionComponent(this, InPosition, InScale)
{
	//set the class id of the ball
	ClassID = RainEntityClassID;

	//set the list of components
	Components.push_back(&RainMeshComponent);
	Components.push_back(&RainVelocityComponent);
	Components.push_back(&RainCollisionComponent);
}
