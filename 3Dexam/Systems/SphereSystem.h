#pragma once
#include "../Components/CollisionNodeComponent.h"
#include "../Entities/BallEntity.h"
#include "../Entities/BoxEntity.h"

class CollisionTreeNode;

class SphereSystem
{
	public:

	//function to check for collisions between container boxes and spheres
	static void CheckCollisions(const BoxEntity* ContainerBox, const std::vector<BallEntity*>& Spheres);

	//helper function to check for collisions between a sphere and a container box
	static bool CheckCollision(const BoxEntity* Box, const BallEntity* Sphere);

	//helper function to check for collisions between two spheres
	static bool CheckCollision(const BallEntity* Sphere1, const BallEntity* Sphere2);

	//helper function to bounce a sphere off a container box
	static void BounceSphere(BallEntity* Sphere, const BoxEntity* Box);

	//helper function to bounce two spheres off each other
	static void BounceSpheres(BallEntity* Sphere1, BallEntity* Sphere2);
};