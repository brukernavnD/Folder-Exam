#pragma once
#include "Entity.h"
#include "../Components/TransformComponent.h"
#include "../Components/RainEmitterComponent.h"

//the class id for the static emitter entity
constexpr int StaticEmitterEntityClassID = 4;

class StaticEmitterEntity : public Entity
{
public:

	//the emitter component of the entity
	RainEmitterComponent EmitterComp;

	//constructor(s)
	StaticEmitterEntity();
	StaticEmitterEntity(const glm::vec3& InPosition, const glm::vec3& InScale);
};

