#include "TransformComponent.h"

TransformComponent::TransformComponent(const glm::vec3 InPos, const glm::vec3 InScale): Position(InPos), Scale(InScale)
{
	
}

TransformComponent::TransformComponent(Entity* InOwner, const glm::vec3 InPos, const glm::vec3 InScale) : Component(InOwner), Position(InPos), Scale(InScale)
{
}

glm::vec3* TransformComponent::GetPosition()
{
	return &Position;
}

glm::vec3* TransformComponent::GetScale()
{
	return &Scale;
}

glm::mat4 TransformComponent::GetTransformMatrix() const
{
	//create the transform matrix
	const glm::mat4 Transform = {
		Scale.x, 0, 0, 0,
		0, Scale.y, 0, 0,
		0, 0, Scale.z, 0,
		Position.x, Position.y, Position.z, 1
	};

	//return the transform matrix
	return Transform;
}
