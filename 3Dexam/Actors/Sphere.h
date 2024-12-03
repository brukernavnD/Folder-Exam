#pragma once
#include "../Core/WorldObject.h"
#include "../Core/Helpers.h"


class CollisionTreeNode;

//the sphere class id
constexpr int SphereClassID = 3;

//the amount of old positions to store
constexpr int SphereMaxIterationsStored = 40;

//the minimum amount of velocity a sphere has to have before stopping updates
constexpr float SphereDataChangeThreshold = 0.6f;

//the minimum timestep between updates to the spheres old data
constexpr float SphereDataMinDeltaTime = 0.05f;

//much of the code for this object was adapted from https://www.songho.ca/opengl/gl_sphere.html
class Sphere_ : public WorldObject
{
public:

	//constructor(s)
	Sphere_(glm::vec3 InPos, glm::vec3 InSize, glm::vec3 InVelocity = glm::vec3(0, 0, 0));

	////vector of the old positions of the sphere
	//std::vector<glm::vec3> OldPositions;

	////vector of the old velocities of the sphere
	//std::vector<glm::vec3> OldVelocities;

	////vector of the old delta times of the sphere
	//std::vector<float> OldDeltas;

	//vector of the old triangles of the sphere
	std::vector<std::vector<Vertex*>> OldTriangles;

	//vector for the time at which the old data was stored
	std::vector<float> OldTimes;

	//storage for whether this sphere can be moved
	bool CanMove = true;

	//override(s)
	static std::vector<Vertex> CreateVertices();
	static std::vector<unsigned int> CreateIndices();
	void Tick(float DeltaTime) override;
};

