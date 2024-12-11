#include "SphereSystem.h"

#include <iostream>
#include <ostream>

#include "../Core/CollisionTreeNode.h"
#include "../Entities/BoxEntity.h"
#include "../Entities/BallEntity.h"

void SphereSystem::CheckCollisions(const BoxEntity* ContainerBox, const std::vector<CollisionNodeComponent*>& Spheres)
{
	//create the collision tree
	const CollisionTreeNode Root = CollisionTreeNode(ContainerBox->TransformComp.Position, ContainerBox->TransformComp.Scale, Spheres);
}

bool SphereSystem::CheckCollision(const BoxEntity* Box, const CollisionNodeComponent* Sphere)
{
    //get the half size of the sphere
    const glm::vec3 SphereHalfSize = Sphere->Scale / 2.0f;

    //get the half size of the container box
    const glm::vec3 BoxHalfSize = Box->TransformComp.Scale / 2.0f;

    //get the distance between the sphere and the container box
    glm::vec3 Distance = Sphere->Position - Box->TransformComp.Position;

    //get the distance between the sphere and the container box
    const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

    //get the closest point on the container box to the sphere
    const glm::vec3 Closest = Box->TransformComp.Position + Clamped;

    //get the distance between the sphere and the closest point on the container box
    Distance = Sphere->Position - Closest;

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

bool SphereSystem::CheckCollision(const CollisionNodeComponent* Sphere1, const CollisionNodeComponent* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->Position - Sphere2->Position;

	////get the length of the distance
	//const float Length = length(Distance);

	//get the length of the distance
	const float Length = sqrt(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//check if the length is less than or equal to the sum of the radii of the two spheres
	if (Length <= Sphere1->Scale.x + Sphere2->Scale.x)
	{
		//return true
		return true;
	}

	//return false
	return false;
}

void SphereSystem::BounceSphere(CollisionNodeComponent* Sphere, const BoxEntity* Box)
{
	//get the half size of the sphere
	const glm::vec3 SphereHalfSize = Sphere->Scale / 2.0f;

	//get the half size of the container box
	const glm::vec3 BoxHalfSize = Box->TransformComp.Scale / 2.0f;

	//get the distance between the sphere and the container box
	glm::vec3 Distance = Sphere->Position - Box->TransformComp.Position;

	//get the distance between the sphere and the container box
	const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

	//get the closest point on the container box to the sphere
	const glm::vec3 Closest = Box->TransformComp.Position + Clamped;

	//get the distance between the sphere and the closest point on the container box
	Distance = Sphere->Position - Closest;

	//get the length of the distance
	const float Length = length(Distance);

	//get the normal of the distance
	const glm::vec3 Normal = normalize(Distance);

	//get the penetration
	const float Penetration = SphereHalfSize.x - Length;

	//get the new position of the sphere
	Sphere->Position += Normal * Penetration;

	//get the velocity component for the sphere
	VelocityComponent* SphereVelocityComponent = Sphere->Owner->GetComponent<VelocityComponent>();

	//get the new velocity of the sphere
	SphereVelocityComponent->Velocity = reflect(SphereVelocityComponent->Velocity, Normal);
}

void SphereSystem::BounceSpheres(CollisionNodeComponent* Sphere1, CollisionNodeComponent* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->Position - Sphere2->Position;

	//get the length of the distance
	const float Length = length(Distance);

	//get the normal of the distance
	const glm::vec3 Normal = normalize(Distance);

	//get the penetration
	const float Penetration = Sphere1->Scale.x + Sphere2->Scale.x - Length;

	//get the new position of the first sphere
	Sphere1->Position += Normal * Penetration;

	//get the new position of the second sphere
	Sphere2->Position -= Normal * Penetration;

	//get the first sphere's velocity component
	VelocityComponent* Sphere1VelocityComponent = Sphere1->Owner->GetComponent<VelocityComponent>();

	//get the second sphere's velocity component
	VelocityComponent* Sphere2VelocityComponent = Sphere2->Owner->GetComponent<VelocityComponent>();

	//get the new velocity of the first sphere
	Sphere1VelocityComponent->Velocity = reflect(Sphere1VelocityComponent->Velocity, Normal);

	//get the new velocity of the second sphere
	Sphere2VelocityComponent->Velocity = reflect(Sphere2VelocityComponent->Velocity, -Normal);
}

CollisionTreeNode SphereSystem::ConstructCollisionTree(const glm::vec3& InPosition, const glm::vec3& InHalfSize, const std::vector<CollisionNodeComponent*> InSpheres, CollisionTreeNode* InParent)
{
	//create a new collision tree node
	CollisionTreeNode Node = CollisionTreeNode(InPosition, InHalfSize, InSpheres, InParent);

	//check if the node is a leaf node
	if (Node.IsLeaf())
	{
		//return the node
		return Node;
	}

	//get the half size of the node
	const glm::vec3 HalfSize = InHalfSize / 2.0f;

	//get the children of the node
	std::vector<CollisionTreeNode> Children;

	//iterate through all the children
	for (int I = 0; I < CollisionTreeNode::TreeType; ++I)
	{
		//get the position of the child
		const glm::vec3 Position = InPosition + HalfSize * glm::vec3((I & 1) ? 1.0f : -1.0f, (I & 2) ? 1.0f : -1.0f, (I & 4) ? 1.0f : -1.0f);

		//create a new collision tree node
		CollisionTreeNode Child = ConstructCollisionTree(Position, HalfSize, InSpheres, &Node);

		//add the child to the children
		Children.push_back(Child);
	}

	//set the children of the node
	Node.Children = Children;

	//return the node
	return Node;
}
