#pragma once
#include "CollisionComponent.h"

class CollisionNodeComponent : public CollisionComponent
{
public:

	//debug string
	std::string DebugString = "Collision Node";

	//constructor(s)
	CollisionNodeComponent(Entity* InOwner, glm::vec3 InPosition, glm::vec3 InScale);
};

