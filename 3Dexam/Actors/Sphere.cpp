#include "Sphere.h"

#include <iostream>

#include "../Core/Helpers.h"
#include "../Core/World.h"

constexpr int Segments = 16;
constexpr float PI = 3.14159265359f;

Sphere_::Sphere_(glm::vec3 InPos, glm::vec3 InSize, glm::vec3 InVelocity) : WorldObject(InPos, InSize, InVelocity, "Sphere (" + std::to_string(GameWorld.CurrentValidID + 1) + ")")
{
	//set the class id
	ClassID = SphereClassID;

	//set the velocity
	Velocity = InVelocity;
}

std::vector<Vertex> Sphere_::CreateVertices()
{
	//storage for the vertices
	std::vector<Vertex> Vertices;

	//reserve the space for the vertices
	Vertices.reserve((Segments + 1) * (Segments + 1));

	for (int y = 0; y <= Segments; ++y)
	{
		for (int x = 0; x <= Segments; ++x)
		{
			const float xSegment = static_cast<float>(x) / static_cast<float>(Segments);
			const float ySegment = static_cast<float>(y) / static_cast<float>(Segments);
			const float xPos = std::cos(xSegment * 2 * PI) * std::sin(ySegment * PI);
			const float yPos = std::cos(ySegment * PI);
			const float zPos = std::sin(xSegment * 2 * PI) * std::sin(ySegment * PI);

			Vertex Vertex;
			Vertex.Position = glm::vec3(xPos, yPos, zPos);
			Vertex.Normal = glm::vec3(xPos, yPos, zPos);
			Vertex.TexCoords = glm::vec2(xSegment, ySegment);
			Vertices.push_back(Vertex);
		}
	}

	return Vertices;
}

std::vector<unsigned int> Sphere_::CreateIndices()
{
	//storage for the indices
	std::vector<unsigned int> Indices;

	//reserve the space for the indices
	Indices.reserve(Segments * Segments * 6);

	for (int y = 0; y < Segments; ++y)
	{
		for (int x = 0; x < Segments; ++x)
		{
			Indices.push_back(y * (Segments + 1) + x);
			Indices.push_back((y + 1) * (Segments + 1) + x);
			Indices.push_back(y * (Segments + 1) + x + 1);

			Indices.push_back(y * (Segments + 1) + x + 1);
			Indices.push_back((y + 1) * (Segments + 1) + x);
			Indices.push_back((y + 1) * (Segments + 1) + x + 1);
		}
	}

	return Indices;
}

void Sphere_::Tick(float DeltaTime)
{
	//call the parent implementation
	WorldObject::Tick(DeltaTime);

	////check if the old positions vector is full
	//if (OldPositions.size() >= SphereMaxOldPositions)
	//{
	//	//erase the first element
	//	OldPositions.erase(OldPositions.begin());
	//}

	////check if the old velocities vector is full
	//if (OldVelocities.size() >= SphereMaxOldPositions)
	//{
	//	//erase the first element
	//	OldVelocities.erase(OldVelocities.begin());
	//}

	////check if the old delta times vector is full
	//if (OldDeltas.size() >= SphereMaxOldPositions)
	//{
	//	//erase the first element
	//	OldDeltas.erase(OldDeltas.begin());
	//}

	////add the current position to the old positions
	//OldPositions.push_back(Position);

	////add the current velocity to the old velocities
	//OldVelocities.push_back(Velocity);

	////add the current delta time to the old delta times
	//OldDeltas.push_back(DeltaTime);

	//check if the we can move
	if (CanMove)
	{
		//perform the movement
		Position += Velocity * DeltaTime;
	}
}
