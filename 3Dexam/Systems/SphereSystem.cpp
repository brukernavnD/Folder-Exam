#include "SphereSystem.h"

#include <iostream>
#include <ostream>

#include "../Core/CollisionTreeNode.h"
#include "../Entities/BoxEntity.h"
#include "../Entities/BallEntity.h"

void SphereSystem::CheckCollisions(const BoxEntity* ContainerBox, const std::vector<BallEntity*>& Spheres)
{
	//create the collision tree
	const CollisionTreeNode Root = CollisionTreeNode(ContainerBox->TransformComp.Position, ContainerBox->TransformComp.Scale, Spheres);
}

bool SphereSystem::CheckCollision(const BoxEntity* Box, const BallEntity* Sphere)
{
    //get the half size of the sphere
    const glm::vec3 SphereHalfSize = Sphere->BallCollisionComponent.Scale / 2.0f;

    //get the half size of the container box
    const glm::vec3 BoxHalfSize = Box->TransformComp.Scale / 2.0f;

    //get the distance between the sphere and the container box
    glm::vec3 Distance = Sphere->BallCollisionComponent.Position - Box->TransformComp.Position;

    //get the distance between the sphere and the container box
    const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

    //get the closest point on the container box to the sphere
    const glm::vec3 Closest = Box->TransformComp.Position + Clamped;

    //get the distance between the sphere and the closest point on the container box
    Distance = Sphere->BallCollisionComponent.Position - Closest;

	//get the length of the distance
	const float Length = sqrt(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

    //check if the length is less than or equal to the radius of the sphere
    if (Length + SphereHalfSize.x / 2 < SphereHalfSize.x)
    {
        //return true
        return true;
    }

    //return false
    return false;
}

bool SphereSystem::CheckCollision(const BallEntity* Sphere1, const BallEntity* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->BallCollisionComponent.Position - Sphere2->BallCollisionComponent.Position;

	////get the length of the distance
	//const float Length = length(Distance);

	//get the length of the distance
	const float Length = sqrt(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//check if the length is less than or equal to the sum of the radii of the two spheres
	if (Length <= Sphere1->BallCollisionComponent.Scale.x + Sphere2->BallCollisionComponent.Scale.x)
	{
		//return true
		return true;
	}

	//return false
	return false;
}

void SphereSystem::BounceSphere(BallEntity* Sphere, const BoxEntity* Box)
{
	//get the half size of the sphere
	const glm::vec3 SphereHalfSize = Sphere->BallCollisionComponent.Scale / 2.0f;

	//get the half size of the container box
	const glm::vec3 BoxHalfSize = Box->TransformComp.Scale / 2.0f;

	//get the distance between the sphere and the container box
	glm::vec3 Distance = Sphere->BallCollisionComponent.Position - Box->TransformComp.Position;

	//get the distance between the sphere and the container box
	const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

	//get the closest point on the container box to the sphere
	const glm::vec3 Closest = Box->TransformComp.Position + Clamped;

	//get the distance between the sphere and the closest point on the container box
	Distance = Sphere->BallCollisionComponent.Position - Closest;

	//get the length of the distance
	const float Length = length(Distance);

	//get the normal of the distance
	const glm::vec3 Normal = normalize(Distance);

	//get the penetration
	const float Penetration = SphereHalfSize.x - Length;

	//get the new position of the sphere
	Sphere->BallCollisionComponent.Position += Normal * Penetration;

	//get the new velocity of the sphere
	Sphere->BallVelocityComponent.SetVelocity(reflect(Sphere->BallVelocityComponent.Velocity, Normal));
}

void SphereSystem::BounceSpheres(BallEntity* Sphere1, BallEntity* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->BallCollisionComponent.Position - Sphere2->BallCollisionComponent.Position;

	//get the length of the distance
	const float Length = length(Distance);

	//get the normal of the distance
	const glm::vec3 Normal = normalize(Distance);

	//get the penetration
	const float Penetration = Sphere1->BallCollisionComponent.Scale.x + Sphere2->BallCollisionComponent.Scale.x - Length;

	//get the new position of the first sphere
	Sphere1->BallCollisionComponent.Position += Normal * Penetration;

	//get the new position of the second sphere
	Sphere2->BallCollisionComponent.Position -= Normal * Penetration;

	//get the new velocity of the first sphere
	Sphere1->BallVelocityComponent.SetVelocity(reflect(Sphere1->BallVelocityComponent.Velocity, Normal));

	//get the new velocity of the second sphere
	Sphere2->BallVelocityComponent.SetVelocity(reflect(Sphere2->BallVelocityComponent.Velocity, -Normal));
}