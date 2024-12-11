#include "BallEntity.h"

BallEntity::BallEntity()
{
	//set the class id of the ball
	ClassID = BallEntityClassID;
}

BallEntity::BallEntity(glm::vec3 InPosition, glm::vec3 InScale, glm::vec3 InVelocity, glm::vec3 InAcceleration) : Entity("BallEntity"), BallMeshComponent(this, SphereRenderingID), BallVelocityComponent(this, InVelocity, InAcceleration), BallCollisionComponent(this, InPosition, InScale)
{
	//set the class id of the ball
	ClassID = BallEntityClassID;

	//set the list of components
	Components.push_back(&BallMeshComponent);
	Components.push_back(&BallVelocityComponent);
	Components.push_back(&BallCollisionComponent);
}
