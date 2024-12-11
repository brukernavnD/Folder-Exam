#pragma once
#include "Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"

//the class id for the box entity
constexpr int BoxEntityClassID = 5;

class BoxEntity : public Entity
{
public:

	//the transform component of the entity
	TransformComponent TransformComp;

	//the mesh component of the entity
	MeshComponent MeshComp;

	//constructor(s)
	BoxEntity();
	explicit BoxEntity(glm::vec3 InPosition = glm::vec3(0), glm::vec3 InScale = glm::vec3(1));
};

