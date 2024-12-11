#pragma once
#include <string>
#include <vector>

#include "../Components/CameraComponent.h"
#include "RenderingSystem.h"
#include "../Entities/Player.h"

class PlayerControllerComponent;
class SphereRenderingData;
class BoxRenderingData;
struct Vertex;
class Entity;


class WorldSystem
{
public:

	//constructor(s)
	WorldSystem();

private:

	//array to store pointers all world objects (houses, npcs, pickups, player character, etc.)
	std::vector<Entity*> WorldObjects = {};

	//the rendering helper
	RenderingSystem Renderer = RenderingSystem();

	//the player for the world
	Player WorldPlayer = Player();

public:

	//the current lowest valid id for any new world object
	int CurrentValidID = 0;

	//function to initialize the world
	void InitializeWorld();

	//function to tick the world
	void TickWorld(float DeltaTime);

	//function to update the world (tick and render)
	void UpdateWorld(float InDeltaTime);

	//function to spawn a new world object
	template<typename T, typename... Args>
	T* SpawnObject(Args... InArgs);

	//function to destroy an object
	template<typename T>
	void DestroyObject(T* InEntity);

	/** getters and setters */

	//function to get all the world objects
	std::vector<Entity*>& GetWorldObjects();

	//function to get a pointer to the current player camera
	CameraComponent* GetPlayerCamera();

	//function to get a pointer to the current player controller
	PlayerControllerComponent* GetPlayerController();

	//function to get a pointer to the rendering system of the world
	RenderingSystem* GetRenderingSystem();

	//template function to get an entity of a specific type
	template<typename InEntityClass>
	InEntityClass* GetEntityOfClass();

	//template function to get all entities of a specific type
	template<typename InEntityClass>
	std::vector<InEntityClass*> GetEntitiesOfClass();
};

template <typename T, typename ... Args>
T* WorldSystem::SpawnObject(Args... InArgs)
{
	//create a new object
	T* NewObject = new T(InArgs...);

	//set the object's id
	NewObject->ID = CurrentValidID;

	//increment the current valid id
	CurrentValidID++;

	//add the object to the world objects
	WorldObjects.push_back(NewObject);

	//return the new object
	return NewObject;
}

template <typename T>
void WorldSystem::DestroyObject(T* InEntity)
{
	//create the iterator
	const std::vector<Entity*>::iterator it = std::find(WorldObjects.begin(), WorldObjects.end(), InEntity);

	//check if we're not at the and of the world objects array
	if (it != WorldObjects.end())
	{
		//erase the object
		WorldObjects.erase(it);
	}

	//delete the object
	delete InEntity;
}

template <typename InEntityClass>
InEntityClass* WorldSystem::GetEntityOfClass()
{
	//iterate over the world objects
	for (Entity* Object : WorldObjects)
	{
		//try to cast the object to the desired type
		InEntityClass* CastedObject = dynamic_cast<InEntityClass*>(Object);

		//if the cast was successful, return the object
		if (CastedObject)
		{
			//return the object
			return CastedObject;
		}
	}

	//return null if the object was not found
	return nullptr;
}

template <typename InEntityClass>
std::vector<InEntityClass*> WorldSystem::GetEntitiesOfClass()
{
	//create a vector to store the objects
	std::vector<InEntityClass*> Objects;

	//iterate over the world objects
	for (Entity* Object : WorldObjects)
	{
		//try to cast the object to the desired type
		InEntityClass* CastedObject = dynamic_cast<InEntityClass*>(Object);

		//if the cast was successful, add the object to the vector
		if (CastedObject)
		{
			//add the object to the vector
			Objects.push_back(CastedObject);
		}
	}

	//return the vector of objects
	return Objects;
}
