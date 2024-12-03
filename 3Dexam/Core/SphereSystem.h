#pragma once
#include "Helpers.h"

class PointCloudSurface_;
class CollisionTreeNode;
class Sphere_;
class ContainerBox_;

//the spheres mass
constexpr float SphereMass = 4;

class SphereSystem
{
	public:

	//function to check for collisions between container boxes and spheres
	static void CheckCollisions(const std::vector<WorldObject*>& Objects, float InDeltaTime);

	//helper function to check for collisions between a sphere and a container box
	static bool CheckCollision(const ContainerBox_* Box, const Sphere_* Sphere);

	//helper function to check for collisions between a sphere and a point cloud surface
	static bool IsSphereInsidePointCloudSurface(const Sphere_* Sphere);

	//helper function to check for collisions between two spheres
	static bool CheckCollision(const Sphere_* Sphere1, const Sphere_* Sphere2);

	//helper function to bounce a sphere off a container box
	static void BounceSphere(Sphere_* Sphere, const ContainerBox_* Box);

	//helper function to bounce two spheres off each other
	static void BounceSpheres(Sphere_* Sphere1, Sphere_* Sphere2);

	//function to get the triangle below a sphere inside the bounds of the point cloud surface
	static std::vector<Vertex*> UpdateSphereAndGetTriangleBelow(Sphere_* Sphere, float InDeltaTime);
};