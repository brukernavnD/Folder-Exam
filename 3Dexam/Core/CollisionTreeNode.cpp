#include "CollisionTreeNode.h"

#include <iostream>
#include <ostream>

#include "SphereSystem.h"
#include "World.h"
#include "../Actors/Sphere.h"

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

CollisionTreeNode::CollisionTreeNode(const glm::vec3 InPosition, const glm::vec3 InHalfSize, const std::vector<Sphere_*>& InSpheres, const float InDeltaTime, CollisionTreeNode* InParent) : Parent(InParent), Position(InPosition), HalfSize(InHalfSize)
{
	////print the position of the node
	//std::cout << "Position: " << Position.x << ", " << Position.y << ", " << Position.z << '\n';

	//check if the amount of spheres is less than or equal to the maximum amount of spheres
	if (InSpheres.size() <= MaxSpheres)
	{
		//add the spheres to the node
		std::copy(InSpheres.begin(), InSpheres.end(), std::back_inserter(Spheres));

		//iterate through all spheres
		for (int i = 0; i < this->Spheres.size(); i++)
		{
			////check if the sphere is colliding with the container box
			//if (!CollisionHelper::CheckCollision(GameWorld.Boxes[0], Spheres[i]))
			//{
			//	//bounce the sphere off the container box
			//	CollisionHelper::BounceSphere(Spheres[i], GameWorld.Boxes[0]);
			//}

			//check if the sphere is inside the point cloud
			if (SphereSystem::IsSphereInsidePointCloudSurface(Spheres[i]) && Spheres[i]->CanMove)
			{
				//get the triangle below the sphere
				std::vector<Vertex*> Triangle = SphereSystem::UpdateSphereAndGetTriangleBelow(Spheres[i], InDeltaTime);

				//check if the triangle is valid
				if (!Triangle.empty())
				{
					//get the normal of the triangle
					const glm::vec3 Normal = normalize(cross(Triangle[1]->Position - Triangle[0]->Position, Triangle[2]->Position - Triangle[0]->Position));

					//get the gravity force projected onto the normal of the triangle
					const glm::vec3 ProjectedGravity = 9.81f * SphereMass * glm::vec3(Normal.x * Normal.y, Normal.y * Normal.y - 1, Normal.z * Normal.y);

					//add the projected gravity force for this sphere
					Spheres[i]->Velocity += ProjectedGravity * InDeltaTime;

					//the average friction of the vertices
					const float Friction = (Triangle[0]->Friction + Triangle[1]->Friction + Triangle[2]->Friction) / 3;

					//reduce the velocity of the sphere by the friction
					Spheres[i]->Velocity -= Spheres[i]->Velocity * InDeltaTime * Friction;
				}

				////storage for the elapsed time todo: replace with the actual elapsed time since program start
				//const float ElapsedTime = GL_TIME_ELAPSED;

				////check if the sphere dosn't have enough old data
				//if (Spheres[i]->OldTriangles.size() < SphereMaxIterationsStored)
				//{
				//	//check if the sphere has any old data
				//	if (Spheres[i]->OldTriangles.empty())
				//	{
				//		//add the triangle to the sphere's vector of triangles
				//		Spheres[i]->OldTriangles.push_back(Triangle);

				//		//add the current time to the sphere's vector of times
				//		Spheres[i]->OldTimes.push_back(ElapsedTime);
				//	}
				//	//check if enough time has passed to update the old data of the sphere
				//	else if (ElapsedTime - Spheres[i]->OldTimes.back() > SphereDataMinDeltaTime)
				//	{
				//		//add the triangle to the sphere's vector of triangles
				//		Spheres[i]->OldTriangles.push_back(Triangle);

				//		//add the current time to the sphere's vector of times
				//		Spheres[i]->OldTimes.push_back(ElapsedTime);
				//	}
				//}
				//else
				//{
				//	//check if enough time has passed to update the old data of the sphere
				//	if (ElapsedTime - Spheres[i]->OldTimes.back() > SphereDataMinDeltaTime)
				//	{
				//		//check if the sphere's vector of triangles is full
				//		if (Spheres[i]->OldTriangles.size() >= SphereMaxIterationsStored)
				//		{
				//			//remove the first triangle
				//			Spheres[i]->OldTriangles.erase(Spheres[i]->OldTriangles.begin());

				//			//remove the first time
				//			Spheres[i]->OldTimes.erase(Spheres[i]->OldTimes.begin());
				//		}

				//		//add the triangle to the sphere's vector of triangles
				//		Spheres[i]->OldTriangles.push_back(Triangle);

				//		//add the current time to the sphere's vector of times
				//		Spheres[i]->OldTimes.push_back(GL_TIME_ELAPSED);
				//	}
				//}
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
	std::vector<std::vector<Sphere_*>> SpheresToAssign(TreeType);

	//allocate space for the spheres to assign
	for (int i = 0; i < TreeType; i++)
	{
		SpheresToAssign[i].reserve(InSpheres.size());
	}

	//iterate through all spheres
	for (Sphere_* Sphere : InSpheres)
	{
		//check if the sphere's position has become invalid
		if (any(isnan(Sphere->Position)))
		{
			//print the position of the sphere
			std::cout << "Sphere: " << Sphere->Position.x << ", " << Sphere->Position.y << ", " << Sphere->Position.z << '\n';

			//continue to the next sphere
			continue;
		}

		//bool for whether or not the sphere has been assigned to a child
		bool SphereAssigned = false;

		//check if the sphere is to be assigned to the bottom left child
		if (Sphere->Position.x <= Position.x && Sphere->Position.z <= Position.z)
		{
			//add the sphere to the bottom left child
			SpheresToAssign[0].push_back(Sphere);
			SphereAssigned = true;
		}

		//check if the sphere is to be assigned to the bottom right child
		if (Sphere->Position.x > Position.x && Sphere->Position.z <= Position.z)
		{
			//add the sphere to the bottom right child
			SpheresToAssign[1].push_back(Sphere);
			SphereAssigned = true;
		}

		//check if the sphere is to be assigned to the top left child
		if (Sphere->Position.x <= Position.x && Sphere->Position.z > Position.z)
		{
			//add the sphere to the top left child
			SpheresToAssign[2].push_back(Sphere);
			SphereAssigned = true;
		}

		//check if the sphere is to be assigned to the top right child
		if (Sphere->Position.x > Position.x && Sphere->Position.z > Position.z)
		{
			//add the sphere to the top right child
			SpheresToAssign[3].push_back(Sphere);
			SphereAssigned = true;
		}

		////check if the sphere has not been assigned to a child
		//if (!SphereAssigned)
		//{
		//	//print the position of the sphere
		//	std::cout << "Sphere: " << Sphere->Position.x << ", " << Sphere->Position.y << ", " << Sphere->Position.z << std::endl << std::endl;

		//	//add the sphere to the node
		//	this->Spheres.push_back(Sphere);
		//}
	}

	//create the children
	for (int i = 0; i < TreeType; i++)
	{
		//get the offset for the child
		const glm::vec3 Offset = HalfSize * glm::vec3((i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f);

		//get the position of the child
		const glm::vec3 ChildPosition = Position + Offset;

		//add the child to the children
		Children.emplace_back(ChildPosition, HalfSize / 2.f, SpheresToAssign[i], InDeltaTime, this);
	}
}

bool CollisionTreeNode::IsLeaf() const
{
	return Children.empty();
}

bool CollisionTreeNode::CheckCollision(const Sphere_* InSphere) const
{
	//get the distance between the sphere and the node
	const glm::vec3 Distance = InSphere->Position - Position;

	//get the distance between the sphere and the node
	const glm::vec3 DistanceClamped = clamp(Distance, -HalfSize, HalfSize);

	//get the distance between the sphere and the node
	const glm::vec3 ClosestPoint = Position + DistanceClamped;

	//get the distance between the sphere and the node
	const glm::vec3 ClosestPointDistance = ClosestPoint - InSphere->Position;

	//get the distance between the sphere and the node
	const float Length = sqrt(ClosestPointDistance.x * ClosestPointDistance.x + ClosestPointDistance.y * ClosestPointDistance.y + ClosestPointDistance.z * ClosestPointDistance.z);

	//check if the length is less than or equal to the radius of the sphere
	if (Length <= InSphere->Size.x)
	{
		//return true
		return true;
	}

	//return false
	return false;
}

void CollisionTreeNode::AssignObjects(const std::vector<Sphere_*>& InSpheres)
{
	//check if the node is a leaf node
	if (IsLeaf())
	{
		//add the spheres to the node
		/*Spheres = */std::copy(InSpheres.begin(), InSpheres.end(), Spheres.begin());

		//print the amount of spheres in the node
		std::cout << "Spheres: " << Spheres.size() << '\n';
	}
	else
	{
		//vector to store the spheres that should be assigned to the children (0 = bottom left, 1 = bottom right, 2 = top left, 3 = top right)
		std::vector<std::vector<Sphere_*>> SpheresToAssign(4);

		//iterate through all spheres
		for (Sphere_* Sphere : InSpheres)
		{
			//bool for whether or not the sphere has been assigned to a child
			bool SphereAssigned = false;

			//check if the sphere is to be assigned to the bottom left child
			if (Sphere->Position.x <= Position.x && Sphere->Position.z <= Position.z)
			{
				//add the sphere to the bottom left child
				SpheresToAssign[0].push_back(Sphere);
				SphereAssigned = true;
			}

			//check if the sphere is to be assigned to the bottom right child
			if (Sphere->Position.x > Position.x && Sphere->Position.z <= Position.z)
			{
				//add the sphere to the bottom right child
				SpheresToAssign[1].push_back(Sphere);
				SphereAssigned = true;
			}

			//check if the sphere is to be assigned to the top left child
			if (Sphere->Position.x <= Position.x && Sphere->Position.z > Position.z)
			{
				//add the sphere to the top left child
				SpheresToAssign[2].push_back(Sphere);
				SphereAssigned = true;
			}

			//check if the sphere is to be assigned to the top right child
			if (Sphere->Position.x > Position.x && Sphere->Position.z > Position.z)
			{
				//add the sphere to the top right child
				SpheresToAssign[3].push_back(Sphere);
				SphereAssigned = true;
			}

			//check if the sphere has not been assigned to a child
			if (!SphereAssigned)
			{
				//print the position of the sphere
				std::cout << "Sphere: " << Sphere->Position.x << ", " << Sphere->Position.y << ", " << Sphere->Position.z << '\n' << '\n';

				//add the sphere to the node
				this->Spheres.push_back(Sphere);
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
