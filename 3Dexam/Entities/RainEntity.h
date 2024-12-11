#pragma once
#include "Entity.h"
#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/KillComponent.h"
#include "../Data/SphereRenderingData.h"

//the class id for the rain entity
constexpr int RainEntityClassID = 1;

class RainEntity : public Entity
{
public:
	//constructor(s)
	RainEntity();
	explicit RainEntity(glm::vec3 InPosition, glm::vec3 InScale = glm::vec3(0.1f), glm::vec3 InVelocity = glm::vec3(0, -9.8, 0), glm::vec3 InAcceleration = glm::vec3(0, -9.8, 0));

	//components
	MeshComponent RainMeshComponent = MeshComponent(this, SphereRenderingID);
	VelocityComponent RainVelocityComponent = VelocityComponent(this, glm::vec3(0));
	CollisionComponent RainCollisionComponent = CollisionComponent(this, glm::vec3(0), glm::vec3(1));
	KillComponent RainKillComponent = KillComponent(this, -10.0f);
};

