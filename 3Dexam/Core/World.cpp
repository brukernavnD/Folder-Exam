#include "World.h"

#include <iostream>
#include <ostream>

#include "PlayerController.h"
#include "SphereSystem.h"
#include "WorldObject.h"
#include "../Actors/ContainerBox.h"

class PlayerController_;
class PlayerCharacter_;

World::World() = default;

void World::InitializeWorld()
{
	//iterate through all container boxes
	for (ContainerBox_* Box : Boxes)
	{
		//add the box to the world objects
		WorldObjects.push_back(Box);
	}

	//create the player controller
	PlayerController = new PlayerController_();

	//iterate through all world objects
	for (WorldObject* Object : WorldObjects)
	{
		//begin play for the object
		Object->BeginPlay();
	}
}

void World::TickWorld(const float DeltaTime) const
{
	//tick the player controller first
	PlayerController->Tick(DeltaTime);

	//iterate through all world objects
	for (WorldObject* Object : WorldObjects)
	{
		//tick the object
		Object->Tick(DeltaTime);
	}
}

void World::UpdateWorld(const float InDeltaTime) const
{
	//check for collisions
	SphereSystem::CheckCollisions(WorldObjects, InDeltaTime);

	//tick the world
	TickWorld(InDeltaTime);

	//render the world
	Renderer.RenderWorldObjects(WorldObjects);
}

std::vector<WorldObject*>& World::GetWorldObjects()
{
	//return the world objects
	return WorldObjects;
}

Camera* World::GetPlayerCamera()
{
	//return a pointer to the player camera
	return &PlayerCamera;
}

PointCloudSurface_* World::GetPointCloudSurface() const
{
	//return the point cloud surface
	return PointCloudSurface;
}
