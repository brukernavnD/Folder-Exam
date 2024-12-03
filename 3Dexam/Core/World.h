#pragma once
#include <string>
#include <vector>

#include "Camera.h"
#include "RenderHelper.h"

class Sphere_;
class PointCloudSurface_;
class ContainerBox_;
struct Vertex;
class PlayerController_;
class WorldObject;


class World
{
public:

	//constructor(s)
	World();

	//array to store pointers all world objects (houses, npcs, pickups, player character, etc.)
	std::vector<WorldObject*> WorldObjects = {};

	//storage for other world objects
	std::vector<WorldObject> OtherWorldObjects = {};

	//storage for container boxes
	std::vector<ContainerBox_*> Boxes = {};

	//the player controller
	PlayerController_* PlayerController = nullptr;

	//the camera
	Camera PlayerCamera = Camera();

	//the rendering helper
	RenderHelper Renderer = RenderHelper();

	//the point cloud surface pointer
	PointCloudSurface_* PointCloudSurface = nullptr;

	//the current lowest valid id for any new world object
	int CurrentValidID = 0;

	//function to initialize the world
	void InitializeWorld();

	//function to tick the world
	void TickWorld(float DeltaTime) const;

	//function to update the world (tick and render)
	void UpdateWorld(float InDeltaTime) const;

	//function to spawn a new world object
	template<typename T>
	T* SpawnObject(glm::vec3 InPosition = glm::vec3(0, 0, 0), glm::vec3 InSize = glm::vec3(1, 1, 1));

	/** getters and setters */

	//function to get all the world objects
	std::vector<WorldObject*>& GetWorldObjects();

	//function to get a pointer to the current player camera
	Camera* GetPlayerCamera();

	//function to get a pointer to the point cloud surface
	PointCloudSurface_* GetPointCloudSurface() const;
};

template <typename T>
T* World::SpawnObject(glm::vec3 InPosition, glm::vec3 InSize)
{
	//create a new object
	T* NewObject = new T(InPosition, InSize);

	//set the object's id
	NewObject->ID = CurrentValidID;

	//increment the current valid id
	CurrentValidID++;

	//add the object to the world objects
	WorldObjects.push_back(NewObject);

	//call the object's begin play function
	NewObject->BeginPlay();

	//return the new object
	return NewObject;
}
