#pragma once
#include "TransformComponent.h"

class CollisionComponent : public TransformComponent
{
public:

	//the collision radius of the object
	float CollisionRadius = 1;

	//constructor(s)
	CollisionComponent() = default;
	explicit CollisionComponent(Entity* parentEntity, glm::vec3 InPosition = glm::vec3(0), glm::vec3 InScale = glm::vec3(1));
};

