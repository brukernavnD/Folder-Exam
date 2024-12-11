#include "VelocityComponent.h"

VelocityComponent::VelocityComponent(Entity* InOwner, const glm::vec3 InVelocity, const glm::vec3 InAcceleration) : Component(InOwner), Velocity(InVelocity), Acceleration(InAcceleration)
{
}

void VelocityComponent::SetVelocity(const glm::vec3& InVelocity)
{
	Velocity = InVelocity;
}
