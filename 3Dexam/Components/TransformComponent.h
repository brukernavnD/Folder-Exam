#pragma once
#include "Component.h"
#include "../Core/Helpers.h"

class TransformComponent : public Component
{
public:

	//constructor(s)
	TransformComponent() = default;
	TransformComponent(glm::vec3 InPos, glm::vec3 InScale);
	TransformComponent(Entity* InOwner, glm::vec3 InPos, glm::vec3 InScale);

	//the position of the object
	glm::vec3 Position = glm::vec3(0.0f);

	//the scale of the object
	glm::vec3 Scale = glm::vec3(1.0f);

	//function to get a pointer to the position of the object
	glm::vec3* GetPosition();

	//function to get a pointer to the scale of the object
	glm::vec3* GetScale();

	//function to get the transformation matrix of the object
	glm::mat4 GetTransformMatrix() const;
};

