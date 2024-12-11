#pragma once
#include "../Components/CollisionNodeComponent.h"
#include "../Entities/BoxEntity.h"

class CollisionTreeNode;

class SphereSystem
{
	public:

	//function to check for collisions between container boxes and spheres
	static void CheckCollisions(const BoxEntity* ContainerBox, const std::vector<CollisionNodeComponent*>& Spheres);

	//helper function to check for collisions between a sphere and a container box
	static bool CheckCollision(const BoxEntity* Box, const CollisionNodeComponent* Sphere);

	//helper function to check for collisions between two spheres
	static bool CheckCollision(const CollisionNodeComponent* Sphere1, const CollisionNodeComponent* Sphere2);

	//helper function to bounce a sphere off a container box
	static void BounceSphere(CollisionNodeComponent* Sphere, const BoxEntity* Box);

	//helper function to bounce two spheres off each other
	static void BounceSpheres(CollisionNodeComponent* Sphere1, CollisionNodeComponent* Sphere2);

	//function to construct a collision tree
	static CollisionTreeNode ConstructCollisionTree(const glm::vec3& InPosition, const glm::vec3& InHalfSize, std::vector<CollisionNodeComponent*> InSpheres, CollisionTreeNode* InParent = nullptr);
};