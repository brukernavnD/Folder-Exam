#pragma once
#include "Entity.h"
#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/CollisionNodeComponent.h"
#include "../Data/SphereRenderingData.h"

//the class id for the ball
constexpr int BallEntityClassID = 2;

class BallEntity : public Entity
{
public:
	//constructor(s)
	BallEntity();
	explicit BallEntity(glm::vec3 InPosition, glm::vec3 InScale = glm::vec3(1), glm::vec3 InVelocity = glm::vec3(0, 10, 0), glm::vec3 InAcceleration = glm::vec3(0));

	//components
	MeshComponent BallMeshComponent = MeshComponent(this, SphereRenderingID);
	VelocityComponent BallVelocityComponent = VelocityComponent(this, glm::vec3(0));
	CollisionNodeComponent BallCollisionComponent = CollisionNodeComponent(this, glm::vec3(0), glm::vec3(1));
};


