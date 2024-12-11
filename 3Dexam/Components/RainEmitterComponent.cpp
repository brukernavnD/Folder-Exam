#include "RainEmitterComponent.h"

#include "../Systems/WorldSystem.h"
#include "../Data/SphereRenderingData.h"
#include "../Entities/RainEntity.h"

RainEmitterComponent::RainEmitterComponent(Entity* InOwner, glm::vec3 InPosition, glm::vec3 InScale): TransformComponent(InOwner, InPosition, InScale)
{
	
}

void RainEmitterComponent::EmitParticles(const int& NumExists) const
{
	//get the amount of particles we want to spawn
	const int NumParticlesToSpawn = ParticleCount - NumExists;

	//iterate through the number of particles we want to spawn
	for (int Index = 0; Index < NumParticlesToSpawn; ++Index)
	{
		//get the random position to spawn the particle in
		const glm::vec3 ParticlePosition = Position + glm::vec3(RandomFloat(-Scale.x * 5, Scale.x * 5), RandomFloat(0, Scale.y * 10), RandomFloat(-Scale.z * 5, Scale.z * 5));

		//spawn the particle
		GetWorld()->SpawnObject<RainEntity>(ParticlePosition, ParticleSize);
	}
}
