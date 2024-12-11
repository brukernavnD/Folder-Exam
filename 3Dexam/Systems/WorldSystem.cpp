#include "WorldSystem.h"

//data includes
#include "../Data/BoxRenderingData.h"
#include "../Data/SphereRenderingData.h"
#include "../Data/PlaneRenderingData.h"

//system includes
#include "KillSystem.h"
#include "SphereSystem.h"
#include "VelocitySystem.h"

//component includes
#include "../Components/PlayerControllerComponent.h"

//entity includes
#include "../Entities/BallEntity.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include "../Entities/StaticEmitterEntity.h"
#include "../Entities/RainEntity.h"


class PlayerControllerComponent;
class PlayerCharacter_;

WorldSystem::WorldSystem() = default;

void WorldSystem::InitializeWorld()
{
	//iterate over the valid rendering id
	for (int ClassID = 0; ClassID <= PlaneRenderingID; ClassID++)
	{
		//check if the rendering id matches the sphere rendering data
		if (ClassID == SphereRenderingID)
		{
			//add the buffers for the sphere rendering data
			Renderer.AddBuffers(SphereRenderingData::CreateVertices(), SphereRenderingData::CreateIndices());
		}

		//check if the rendering id matches the box rendering data
		if (ClassID == BoxRenderingID)
		{
			//add the buffers for the box rendering data
			Renderer.AddBuffers(BoxRenderingData::CreateVertices(), BoxRenderingData::CreateIndices());
		}

		//check if the rendering id matches the plane rendering data
		if (ClassID == PlaneRenderingID)
		{
			//add the buffers for the plane rendering data
			Renderer.AddBuffers(PlaneRenderingData::CreateVertices(), PlaneRenderingData::CreateIndices());
		}

	}

	////iterate through all world objects
	//for (Entity* Object : WorldObjects)
	//{
	//	//begin play for the object
	//	Object->BeginPlay();
	//}
}

void WorldSystem::TickWorld(const float DeltaTime)
{
	//tick the player controller first
	GetPlayerController()->Tick(DeltaTime);

	//update the entities that have a velocity component
	VelocitySystem::UpdateVelocities(WorldObjects, DeltaTime);

	//get all static emitter entities
	const std::vector<StaticEmitterEntity*> StaticEmitters = GetEntitiesOfClass<StaticEmitterEntity>();

	//get all rain entities
	std::vector<RainEntity*> RainEntities = GetEntitiesOfClass<RainEntity>();

	//iterate through all static emitters
	for (StaticEmitterEntity* StaticEmitter : StaticEmitters)
	{
		//emit particles
		StaticEmitter->EmitterComp.EmitParticles(RainEntities.size());
	}

	//check if any rain entities should be killed
	KillSystem::CheckKill(RainEntities);

	//get all ball entities
	std::vector<BallEntity*> BallEntities = GetEntitiesOfClass<BallEntity>();

	//storage for all CollisionNodeComponents
	std::vector<CollisionNodeComponent*> CollisionNodeComponents;

	//reserve space for the CollisionNodeComponents
	CollisionNodeComponents.reserve(BallEntities.size());

	//iterate over all the ballentities
	for (BallEntity* Entity : BallEntities)
	{
		//add their collision node component to the list
		CollisionNodeComponents.push_back(&Entity->BallCollisioComponent);
	}

	//simulate collision for the ball components
	SphereSystem::CheckCollisions(GetEntityOfClass<BoxEntity>(), CollisionNodeComponents);
}

void WorldSystem::UpdateWorld(const float InDeltaTime)
{
	////check for collisions
	//SphereSystem::CheckCollisions(WorldObjects, InDeltaTime);

	//tick the world
	TickWorld(InDeltaTime);

	//render the world
	Renderer.RenderWorldObjects(WorldObjects);
}

std::vector<Entity*>& WorldSystem::GetWorldObjects()
{
	//return the world objects
	return WorldObjects;
}

CameraComponent* WorldSystem::GetPlayerCamera()
{
	//return a pointer to the player camera
	return &WorldPlayer.PlayerCamera;
}

PlayerControllerComponent* WorldSystem::GetPlayerController()
{
	//return a pointer to the player controller
	return &WorldPlayer.PlayerController;
}

RenderingSystem* WorldSystem::GetRenderingSystem()
{
	//return a pointer to the rendering system
	return &Renderer;
}
