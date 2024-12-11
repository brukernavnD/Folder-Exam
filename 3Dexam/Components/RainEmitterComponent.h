#pragma once
#include "TransformComponent.h"

class RainEmitterComponent : public TransformComponent
{
public:

	//the number of particles to emit
	int ParticleCount = 500;

	//the size of the particles
	glm::vec3 ParticleSize = glm::vec3(0.1, 0.2, 0.1);

	//the default velocity of the particles
	glm::vec3 InitialParticleVelocity = glm::vec3(0, 0, 0);

	//the default acceleration of the particles
	glm::vec3 ParticleAcceleration = glm::vec3(0, -9.8, 0);

	//constructor(s)
	RainEmitterComponent() = default;
	RainEmitterComponent(Entity* InOwner, glm::vec3 InPosition, glm::vec3 InScale);

	//function to emit rain (spheres) from the emitter
	void EmitParticles(const int& NumExists) const;
};

