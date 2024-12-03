#include "SphereSystem.h"

#include <iostream>
#include <ostream>

#include "CollisionTreeNode.h"
#include "World.h"
#include "../Actors/ContainerBox.h"
#include "../Actors/Sphere.h"
#include "../Actors/PointCloudSurface.h"

void SphereSystem::CheckCollisions(const std::vector<WorldObject*>& Objects, const float InDeltaTime)
{
	////iterate through all container boxes
	//for (const ContainerBox_* ContainerBox : ContainerBoxes)
	//{
	//	//iterate through all spheres
	//	for (Sphere_* Sphere : Spheres)
	//	{
	//		//check if the sphere is not moving
	//		if (Sphere->Velocity == glm::vec3(0.0f))
	//		{
	//			//skip the sphere
	//			continue;
	//		}

	//		//check if the sphere is colliding with the container box
	//		if (!CheckCollision(ContainerBox, Sphere))
	//		{
	//			//bounce the sphere off the container box
	//			BounceSphere(Sphere, ContainerBox);
	//		}
	//	}
	//}

	//storage for the spheres
	std::vector<Sphere_*> Spheres;

	//iterate through all world objects
	for (WorldObject* Object : Objects)
	{
		//check if the object is a sphere
		if (Object->ClassID == SphereClassID)
		{
			//add the object to the spheres
			Spheres.push_back(dynamic_cast<Sphere_*>(Object));
		}
	}

	//create a collision tree
	const CollisionTreeNode Root = CollisionTreeNode(CBoxCen, CBoxSize, Spheres, InDeltaTime);

	////assign the spheres to the collision tree
	//Root.AssignObjects(Spheres);

	////solve for collisions in the collision tree
	//Root.SolveCollisions(ContainerBoxes);

	////iterate through all the spheres
	//for (int I = 0; I < Spheres.size(); ++I)
	//{
	//	//iterate through all the spheres
	//	for (int J = I + 1; J < Spheres.size(); ++J)
	//	{
	//		//check if the spheres are colliding with each other
	//		if (CheckCollision(Spheres[I], Spheres[J]))
	//		{
	//			//bounce the spheres off each other
	//			BounceSpheres(Spheres[I], Spheres[J]);
	//		}
	//	}
	//}
}

bool SphereSystem::CheckCollision(const ContainerBox_* Box, const Sphere_* Sphere)
{
    //get the half size of the sphere
    const glm::vec3 SphereHalfSize = Sphere->Size / 2.0f;

    //get the half size of the container box
    const glm::vec3 BoxHalfSize = Box->Size / 2.0f;

    //get the distance between the sphere and the container box
    glm::vec3 Distance = Sphere->Position - Box->Position;

    //get the distance between the sphere and the container box
    const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

    //get the closest point on the container box to the sphere
    const glm::vec3 Closest = Box->Position + Clamped;

    //get the distance between the sphere and the closest point on the container box
    Distance = Sphere->Position - Closest;

    ////get the length of the distance
    //const float Length = length(Distance);

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

bool SphereSystem::IsSphereInsidePointCloudSurface(const Sphere_* Sphere)
{
	//get the point cloud
	const PointCloudSurface_* PointCloud = GetWorld()->GetPointCloudSurface();

	//check if the sphere is further in the -x direction than the point cloud bounds
	if (Sphere->Position.x - Sphere->Size.x / 2 < PointCloud->Corners.at(0).x)
	{
		//return true
		return false;
	}

	//check if the sphere is further in the +x direction than the point cloud bounds
	if (Sphere->Position.x + Sphere->Size.x / 2 > PointCloud->Corners.at(1).x)
	{
		//return true
		return false;
	}

	//check if the sphere is further in the -z direction than the point cloud bounds
	if (Sphere->Position.z - Sphere->Size.x / 2 < PointCloud->Corners.at(0).z)
	{
		//return true
		return false;
	}

	//check if the sphere is further in the +z direction than the point cloud bounds
	if (Sphere->Position.z + Sphere->Size.x / 2 > PointCloud->Corners.at(1).z)
	{
		//return true
		return false;
	}

	//return false
	return true;
}

bool SphereSystem::CheckCollision(const Sphere_* Sphere1, const Sphere_* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->Position - Sphere2->Position;

	//get the length of the distance
	const float Length = sqrt(Distance.x * Distance.x + Distance.y * Distance.y + Distance.z * Distance.z);

	//check if the length is less than or equal to the sum of the radii of the two spheres
	if (Length <= Sphere1->Size.x + Sphere2->Size.x)
	{
		//return true
		return true;
	}

	//return false
	return false;
}

void SphereSystem::BounceSphere(Sphere_* Sphere, const ContainerBox_* Box)
{
	//get the half size of the sphere
	const glm::vec3 SphereHalfSize = Sphere->Size / 2.0f;

	//get the half size of the container box
	const glm::vec3 BoxHalfSize = Box->Size / 2.0f;

	//get the distance between the sphere and the container box
	glm::vec3 Distance = Sphere->Position - Box->Position;

	//get the distance between the sphere and the container box
	const glm::vec3 Clamped = clamp(Distance, -BoxHalfSize, BoxHalfSize);

	//get the closest point on the container box to the sphere
	const glm::vec3 Closest = Box->Position + Clamped;

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

	//get the new velocity of the sphere
	Sphere->Velocity = reflect(Sphere->Velocity, Normal);
}

void SphereSystem::BounceSpheres(Sphere_* Sphere1, Sphere_* Sphere2)
{
	//get the distance between the two spheres
	const glm::vec3 Distance = Sphere1->Position - Sphere2->Position;

	//get the length of the distance
	const float Length = length(Distance);

	//get the normal of the distance
	const glm::vec3 Normal = normalize(Distance);

	//get the penetration
	const float Penetration = Sphere1->Size.x + Sphere2->Size.x - Length;

	//get the new position of the first sphere
	Sphere1->Position += Normal * Penetration;

	//get the new position of the second sphere
	Sphere2->Position -= Normal * Penetration;

	//get the new velocity of the first sphere
	Sphere1->Velocity = reflect(Sphere1->Velocity, Normal);

	//get the new velocity of the second sphere
	Sphere2->Velocity = reflect(Sphere2->Velocity, -Normal);
}

std::vector<Vertex*> SphereSystem::UpdateSphereAndGetTriangleBelow(Sphere_* Sphere, const float InDeltaTime)
{
	//storage for the vertices of the point cloud surface
	std::vector<Vertex*> Vertices = GetWorld()->Renderer.GetVertexPointers(PointCloudSurfaceClassID);

	//iterate through the vertices in sets of 3
	for (int I = 0; I < Vertices.size(); I += 3)
	{
		//get the current triangle
		Vertex& P1 = *Vertices[I];
		Vertex& P2 = *Vertices[I + 1];
		Vertex& P3 = *Vertices[I + 2];

		//storage for the collision point
		Vertex CollisionPoint = Vertex(glm::vec3(0, 0, 0));

		//do a collision check
		if (testRayThruTriangle(P1, P2, P3, Vertex(Sphere->Position), Vertex(Sphere->Position - glm::vec3(0, Sphere->Size.y, 0)), CollisionPoint))
		{
			//get the normal of the triangle
			const glm::vec3 Normal = normalize(cross(P2.Position - P1.Position, P3.Position - P1.Position));

			//get the penetration
			const float Penetration = Sphere->Size.x - length(CollisionPoint.Position - Sphere->Position);

			//storage for the suggested new position of the sphere
			glm::vec3 NewPosition = Sphere->Position + Normal * Penetration;

			////storage for the change in position over the last few frames
			//glm::vec3 SpherePositionChange = glm::vec3(0);

			////storage for the change in velocity over the last few frames
			//glm::vec3 SphereVelocityChange = glm::vec3(0);

			////check if the sphere has enough old positions
			//if (Sphere->OldPositions.size() >= SphereMaxIterationsStored)
			//{
			//	//iterate through the old positions of the sphere
			//	for (int Index2 = 1; Index2 < Sphere->OldPositions.size(); ++Index2)
			//	{
			//		//add the change in position to the sphere position change
			//		SpherePositionChange += abs(Sphere->OldPositions[Index2] - Sphere->OldPositions[Index2 - 1]);

			//		//add the change in velocity to the sphere velocity change
			//		SphereVelocityChange += abs(Sphere->OldVelocities[Index2] - Sphere->OldVelocities[Index2 - 1]);
			//	}
			//}

			////check if the change in position and velocity is negligible and that the sphere had enough old positions
			//if (length(SpherePositionChange) < SphereDataChangeThreshold /*&& length(SphereVelocityChange) < SphereMinVelocity*/ && Sphere->OldPositions.size() == SphereMaxOldPositions)
			//{
			//	//set the new position of the to the one it had in the previous update
			//	Sphere->Position = Sphere->OldPositions.back();

			//	//disable movement for the sphere
			//	Sphere->CanMove = false;

			//	//return an empty vector
			//	return {};
			//}

			//check if the sphere has enough old triangles
			if (Sphere->OldTriangles.size() >= SphereMaxIterationsStored)
			{
				//storage for the amount of unique triangles
				std::vector<std::vector<Vertex*>> UniqueTriangles;

				//iterate through the old triangles of the sphere
				for (const std::vector<Vertex*>& Triangle : Sphere->OldTriangles)
				{
					//check if the triangle is unique
					if (std::find(UniqueTriangles.begin(), UniqueTriangles.end(), Triangle) == UniqueTriangles.end())
					{
						//add the triangle to the unique triangles
						UniqueTriangles.push_back(Triangle);
					}
				}

				//check if the amount of unique triangles is less than 4
				if (UniqueTriangles.size() < 4)
				{
					//disable movement for the sphere
					Sphere->CanMove = false;

					//return an empty vector
					return {};
				}
			}

			//set the new position of the sphere
			Sphere->Position = NewPosition;

			//set the return vertices
			return {Vertices[I], Vertices[I + 1], Vertices[I + 2]};
		}
	}

	//add the gravity force for this sphere
	Sphere->Velocity += glm::vec3(0, -0.981 * SphereMass, 0) * InDeltaTime;

	//iterate through the vertices in sets of 3 again
	for (int I = 0; I < Vertices.size(); I += 3)
	{
		//get the current triangle
		Vertex& P1 = *Vertices[I];
		Vertex& P2 = *Vertices[I + 1];
		Vertex& P3 = *Vertices[I + 2];

		//storage for the collision point
		Vertex CollisionPoint = Vertex(glm::vec3(0, 0, 0));

		//do a collision check
		if (testRayThruTriangle(P1, P2, P3, Vertex(Sphere->Position), Vertex(Sphere->Position - glm::vec3(0, CullingDistance, 0)), CollisionPoint))
		{
			//set the return vertices
			return {Vertices[I], Vertices[I + 1], Vertices[I + 2]};
		}
	}

	//return the vertices
	return {};
}
