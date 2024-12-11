#pragma once
#include "Component.h"
#include "../Core/Helpers.h"

class VelocityComponent : public Component
{
public:

	//the velocity of the owner
	glm::vec3 Velocity = glm::vec3(0);

	//the acceleration of the owner
	glm::vec3 Acceleration = glm::vec3(0, -9.8, 0);

	//constructor(s)
	VelocityComponent() = default;
	explicit VelocityComponent(Entity* InOwner, glm::vec3 InVelocity = glm::vec3(0), glm::vec3 InAcceleration = glm::vec3(0, -9.8, 0));

	//function to set the velocity
	void SetVelocity(const glm::vec3& InVelocity);

};

