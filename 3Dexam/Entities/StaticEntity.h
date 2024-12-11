#pragma once
#include "Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"

//the class id for the static entity
constexpr int StaticEntityClassID = 3;

class StaticEntity : public Entity
{
public:
	//the transform component of the entity
	TransformComponent TransformComp;

	//the mesh component of the entity
	MeshComponent MeshComp;

	//constructor(s)
	StaticEntity();
	explicit StaticEntity(int InRenderingID, glm::vec3 InPosition = glm::vec3(0), glm::vec3 InScale = glm::vec3(1));
};

