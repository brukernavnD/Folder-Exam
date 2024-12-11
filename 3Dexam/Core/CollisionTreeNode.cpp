#include "CollisionTreeNode.h"

#include <iostream>
#include <ostream>

#include "../Components/CollisionNodeComponent.h"
#include "../Entities/BoxEntity.h"
#include "../Entities/BallEntity.h"
#include "../Systems/WorldSystem.h"
#include "../Systems/SphereSystem.h"

CollisionTreeNode::CollisionTreeNode() = default;

CollisionTreeNode::CollisionTreeNode(const glm::vec3 Position, const glm::vec3 HalfSize, const int RemainingDepth, CollisionTreeNode* Parent) : Parent(Parent), Position(Position), HalfSize(HalfSize)
{
	//check if the remaining depth is greater than 0
	if (RemainingDepth <= 0)
	{
		//return
		return;
	}

	//iterate through all children
	for (int i = 0; i < TreeType; i++)
	{
		//create the child nodes
		CollisionTreeNode Child = CollisionTreeNode(Position + glm::vec3(i % 2 * HalfSize.x, 0, i / 2 * HalfSize.z), HalfSize / 2.0f, RemainingDepth - 1, this);

		//add the child to the children
		Children.push_back(Child);
	}

}

CollisionTreeNode::CollisionTreeNode(const glm::vec3 InPosition, const glm::vec3 InHalfSize, const std::vector<BallEntity*>& InSpheres, CollisionTreeNode* InParent) : Parent(InParent), Position(InPosition), HalfSize(InHalfSize)
{
	//check if the amount of spheres is less than or equal to the maximum amount of spheres
	if (InSpheres.size() <= MaxSpheres)
	{
		//add the spheres to the node
		std::copy(InSpheres.begin(), InSpheres.end(), std::back_inserter(Spheres));

		//storage for the box entity
		BoxEntity* Box = GetWorld()->GetEntityOfClass<BoxEntity>();

		//iterate through all spheres
		for (int i = 0; i < this->Spheres.size(); i++)
		{
			//check if the sphere is colliding with the container box
			if (!SphereSystem::CheckCollision(Box, Spheres[i]))
			{
				//bounce the sphere off the container box
				SphereSystem::BounceSphere(Spheres[i], Box);
			}

			//iterate through all other spheres
			for (int j = i + 1; j < this->Spheres.size(); j++)
			{
				//check if the spheres are colliding
				if (SphereSystem::CheckCollision(Spheres[i], Spheres[j]))
				{
					//bounce the spheres off each other
					SphereSystem::BounceSpheres(Spheres[i], Spheres[j]);
				}
			}
		}

		//prevent further execution
		return;
	}

	//vector to store the spheres that should be assigned to the children (0 = bottom left, 1 = bottom right, 2 = top left, 3 = top right)
	std::vector<std::vector<BallEntity*>> SpheresToAssign(TreeType);

	//allocate space for the spheres to assign
	for (int i = 0; i < TreeType; i++)
	{
		SpheresToAssign[i].reserve(InSpheres.size());
	}

	//iterate through all spheres
	for (BallEntity* Sphere : InSpheres)
	{
		//check if the sphere is to be assigned to the bottom left child
		if (Sphere->BallCollisionComponent.Position.x <= Position.x && Sphere->BallCollisionComponent.Position.z <= Position.z)
		{
			//add the sphere to the bottom left child
			SpheresToAssign[0].push_back(Sphere);
		}

		//check if the sphere is to be assigned to the bottom right child
		if (Sphere->BallCollisionComponent.Position.x > Position.x && Sphere->BallCollisionComponent.Position.z <= Position.z)
		{
			//add the sphere to the bottom right child
			SpheresToAssign[1].push_back(Sphere);
		}

		//check if the sphere is to be assigned to the top left child
		if (Sphere->BallCollisionComponent.Position.x <= Position.x && Sphere->BallCollisionComponent.Position.z > Position.z)
		{
			//add the sphere to the top left child
			SpheresToAssign[2].push_back(Sphere);
		}

		//check if the sphere is to be assigned to the top right child
		if (Sphere->BallCollisionComponent.Position.x > Position.x && Sphere->BallCollisionComponent.Position.z > Position.z)
		{
			//add the sphere to the top right child
			SpheresToAssign[3].push_back(Sphere);
		}
	}

	//create the children
	for (int i = 0; i < TreeType; i++)
	{
		//get the offset for the child
		const glm::vec3 Offset = HalfSize * glm::vec3((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f);

		//get the position of the child
		const glm::vec3 ChildPosition = Position + Offset;

		//add the child to the children
		Children.emplace_back(ChildPosition, HalfSize / 2.f, SpheresToAssign[i], this);
	}
}

bool CollisionTreeNode::IsLeaf() const
{
	return Children.empty();
}

bool CollisionTreeNode::CheckCollision(const BallEntity* InSphere) const
{
	//get the distance between the sphere and the node
	const glm::vec3 Distance = InSphere->BallCollisionComponent.Position - Position;

	//get the distance between the sphere and the node
	const glm::vec3 DistanceClamped = clamp(Distance, -HalfSize, HalfSize);

	//get the distance between the sphere and the node
	const glm::vec3 ClosestPoint = Position + DistanceClamped;

	//get the distance between the sphere and the node
	const glm::vec3 ClosestPointDistance = ClosestPoint - InSphere->BallCollisionComponent.Position;

	//get the distance between the sphere and the node
	const float Length = sqrt(ClosestPointDistance.x * ClosestPointDistance.x + ClosestPointDistance.y * ClosestPointDistance.y + ClosestPointDistance.z * ClosestPointDistance.z);

	//check if the length is less than or equal to the radius of the sphere
	if (Length <= InSphere->BallCollisionComponent.Scale.x)
	{
		//return true
		return true;
	}

	//return false
	return false;
}

void CollisionTreeNode::AssignObjects(const std::vector<BallEntity*>& InSpheres)
{
	//check if the node is a leaf node
	if (IsLeaf())
	{
		//add the spheres to the node
		/*Spheres = */std::copy(InSpheres.begin(), InSpheres.end(), Spheres.begin());
	}
	else
	{
		//vector to store the spheres that should be assigned to the children (0 = bottom left, 1 = bottom right, 2 = top left, 3 = top right)
		std::vector<std::vector<BallEntity*>> SpheresToAssign(4);

		//iterate through all spheres
		for (BallEntity* Sphere : InSpheres)
		{

			//check if the sphere is to be assigned to the bottom left child
			if (Sphere->BallCollisionComponent.Position.x <= Position.x && Sphere->BallCollisionComponent.Position.z <= Position.z)
			{
				//add the sphere to the bottom left child
				SpheresToAssign[0].push_back(Sphere);
			}

			//check if the sphere is to be assigned to the bottom right child
			if (Sphere->BallCollisionComponent.Position.x > Position.x && Sphere->BallCollisionComponent.Position.z <= Position.z)
			{
				//add the sphere to the bottom right child
				SpheresToAssign[1].push_back(Sphere);
			}

			//check if the sphere is to be assigned to the top left child
			if (Sphere->BallCollisionComponent.Position.x <= Position.x && Sphere->BallCollisionComponent.Position.z > Position.z)
			{
				//add the sphere to the top left child
				SpheresToAssign[2].push_back(Sphere);
			}

			//check if the sphere is to be assigned to the top right child
			if (Sphere->BallCollisionComponent.Position.x > Position.x && Sphere->BallCollisionComponent.Position.z > Position.z)
			{
				//add the sphere to the top right child
				SpheresToAssign[3].push_back(Sphere);
			}
		}

		//iterate through all children
		for (int i = 0; i < TreeType; i++)
		{
			//check if the child has spheres to assign
			if (!SpheresToAssign[i].empty())
			{
				//assign the spheres to the child
				this->Children[i].AssignObjects(SpheresToAssign[i]);
			}
		}
	}
}
