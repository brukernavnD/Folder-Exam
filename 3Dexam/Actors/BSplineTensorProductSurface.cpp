#include "BSplineTensorProductSurface.h"
#include <ostream>

#include "ContainerBox.h"
#include "PointCloudSurface.h"
#include "../Core/Helpers.h"
#include "../Core/World.h"

BSplineTensorProductSurface_::BSplineTensorProductSurface_(const glm::vec3 InPos, const glm::vec3 InSize) : WorldObject(InPos, InSize, {0,0,0}, "BSplineSurface (" + std::to_string(GameWorld.CurrentValidID + 1) + ")")
{
	ClassID = BSplineTensorProductSurfaceClassID;
}

float BSplineTensorProductSurface_::BasisFunction(const int i, const int Deg, const float u, std::vector<float> Knots)
{
	//check if the degree is 0
	if (Deg == 0)
	{
		//check if the parameter is within the range of the knot
		if (Knots[i] <= u && u < Knots[i + 1])
		{
			//return 1
			return 1;
		}

		//return 0
		return 0;
	}

	//check if i + Deg + 1 is equal to the size of the knots
	if (i + Deg + 1 == Knots.size())
	{
		//check if the parameter is within the range of the knot
		if (Knots[i] <= u && u <= Knots[i + Deg])
		{
			//return 1
			return 1;
		}

		//return 0
		return 0;
	}

	//calculate the basis function
	const float Denom1 = Knots[i + Deg] - Knots[i];
	const float Denom2 = Knots[i + Deg + 1] - Knots[i + 1];

	float FirstPart = 0;
	float SecondPart = 0;

	//check if the denominator is 0
	if (Denom1 != 0)
	{
		FirstPart = ((u - Knots[i]) / Denom1) * BasisFunction(i, Deg - 1, u, Knots);
	}

	//check if the denominator is 0
	if (Denom2 != 0)
	{
		SecondPart = ((Knots[i + Deg + 1] - u) / Denom2) * BasisFunction(i + 1, Deg - 1, u, Knots);
	}

	//return the basis function
	return FirstPart + SecondPart;
}

glm::vec3 BSplineTensorProductSurface_::GetTensorProduct(float u, float v, std::vector<std::vector<glm::vec3>> ControlPoints, std::vector<float> UKnots, std::vector<float> VKnots)
{
	//storage for the tensor product
	glm::vec3 TensorProduct = { 0,0,0 };

	//iterate through each control point
	for (int i = 0; i < ControlPoints.size(); i++)
	{
		for (int j = 0; j < ControlPoints[i].size(); j++)
		{
			//calculate the basis functions
			float BasisU = BasisFunction(i, 2, u, UKnots);
			float BasisV = BasisFunction(j, 2, v, VKnots);

			//add the tensor product
			TensorProduct += ControlPoints[i][j] * BasisU * BasisV;
		}
	}

	//return the tensor product
	return TensorProduct;
}

std::vector<std::vector<glm::vec3>> BSplineTensorProductSurface_::GetControlPoints()
{
	//storage for the control points
	std::vector<std::vector<glm::vec3>> ControlPoints;

	//reserve space for the control points
	ControlPoints.reserve(12);

	//get the vertices from the point cloud surface
	const std::vector<Vertex> Vertices = PointCloudSurface_::CreateVertices();

	//storage for the min and max vertices
	glm::vec3 MaxVertex = glm::vec3(PointCloudSquareSize * (PointCloudGridWidth), 0, PointCloudSquareSize * (PointCloudGridHeight));

	//storage for the points to test for the control points
	std::vector<std::vector<glm::vec3>> TestPoints;

	//reserve space for the test points
	TestPoints.reserve(12);

	//get the largest axis
	const int LargestAxis = MaxVertex.x > MaxVertex.z ? 0 : 2;

	//get the smallest axis
	const int MinimumAxis = LargestAxis == 0 ? 2 : 0;

	//get the length to move on the biggest axis
	const float MaxAxisLength = MaxVertex[LargestAxis];

	//get the length to move on the smallest axis
	const float MinAxisLength = MaxVertex[MinimumAxis];

	//iterate through the smallest axis 3 times
	for (int i = 0; i < 3; i++)
	{
		//storage for the test points
		std::vector<glm::vec3> TestPointsRow;

		//reserve space for the test points
		TestPointsRow.reserve(4);

		//iterate through the larger axis 4 times
		for (int j = 0; j < 4; j++)
		{
			//storage for the min and max positions
			float MaxPos = (j + 1) * MaxAxisLength / 4;
			float MinPos = (i + 1) * MinAxisLength / 3;

			//check if j is 0
			if (j == 0)
			{
				MaxPos = 0;
			}

			//check if i is 0
			if (i == 0)
			{
				MinPos = 0;
			}

			//check if the x axis is the largest
			if (LargestAxis == 0)
			{
				//add the test point
				TestPointsRow.emplace_back(MaxPos, 0, MinPos);
			}

			//check if the z axis is the largest
			if (LargestAxis == 2)
			{
				//add the test point
				TestPointsRow.emplace_back(MinPos, 0, MaxPos);
			}
		}

		//add the test points row
		TestPoints.emplace_back(TestPointsRow);
	}

	//iterate through the test point rows
	for (const std::vector<glm::vec3>& TestPointRow : TestPoints)
	{
		//storage for the control points
		std::vector<glm::vec3> ControlPointsRow;

		//iterate through the test points
		for (const glm::vec3& TestPoint : TestPointRow)
		{
			//storage for the collision point
			Vertex CollisionPoint = Vertex(TestPoint);

			//storage for whether or not we found a collision
			bool FoundCollision = false;

			//iterate through the vertices in sets of 3
			for (int i = 0; i < Vertices.size(); i += 3)
			{
				//get the current triangle
				const Vertex& P1 = Vertices[i];
				const Vertex& P2 = Vertices[i + 1];
				const Vertex& P3 = Vertices[i + 2];

				//check if P1 is equal to the test point in the x and z axis
				if (P1.Position.x == TestPoint.x && P1.Position.z == TestPoint.z)
				{
					//add the point to the control points
					ControlPointsRow.emplace_back(P1.Position);

					//set found collision to true
					FoundCollision = true;

					//break
					break;
				}

				//check if P2 is equal to the test point in the x and z axis
				if (P2.Position.x == TestPoint.x && P2.Position.z == TestPoint.z)
				{
					//add the point to the control points
					ControlPointsRow.emplace_back(P2.Position);

					//set found collision to true
					FoundCollision = true;

					//break
					break;
				}

				//check if P3 is equal to the test point in the x and z axis
				if (P3.Position.x == TestPoint.x && P3.Position.z == TestPoint.z)
				{
					//add the point to the control points
					ControlPointsRow.emplace_back(P3.Position);

					//set found collision to true
					FoundCollision = true;

					//break
					break;
				}

				//create a copy of the test point
				glm::vec3 LocTestPoint = TestPoint;

				//check if the x value of the loc test point is 0
				if (LocTestPoint.x == 0)
				{
					//set the x value to 0.1
					LocTestPoint.x = 0.1;
				}

				//check if the x value of the loc test point is equal to the max vertex x value
				if (LocTestPoint.x == MaxVertex.x)
				{
					//set the x value to the max vertex x value - 0.1
					LocTestPoint.x = MaxVertex.x - 0.1;
				}

				//check if the z value of the loc test point is 0
				if (LocTestPoint.z == 0)
				{
					//set the z value to 0.1
					LocTestPoint.z = 0.1;
				}

				//check if the z value of the loc test point is equal to the max vertex z value
				if (LocTestPoint.z == MaxVertex.z)
				{
					//set the z value to the max vertex z value - 0.1
					LocTestPoint.z = MaxVertex.z - 0.1;
				}

				//do a collision test
				if (testRayThruTriangle(P1, P2, P3, Vertex(LocTestPoint + glm::vec3(0, 200, 0)), Vertex(LocTestPoint + glm::vec3(0, -200, 0)), CollisionPoint))
				{
					//add the collision point to the control points
					ControlPointsRow.emplace_back(CollisionPoint.Position);

					//set found collision to true
					FoundCollision = true;

					//break
					break;
				}
			}

			//check if we didn't find a collision
			if (!FoundCollision)
			{
				//add the test point to the control points
				ControlPointsRow.emplace_back(TestPoint);
			}
		}

		//add the control points row
		ControlPoints.emplace_back(ControlPointsRow);
	}

	//return the control points
	return ControlPoints;
}

std::vector<Vertex> BSplineTensorProductSurface_::CreateVertices(std::vector<std::vector<glm::vec3>> ControlPoints, std::vector<float> UKnots, std::vector<float> VKnots)
{
	//storage for the vertices
	std::vector<Vertex> ReturnVertices;

	//iterate through the u values
	for (float u = 0; u <= 1; u += 0.1)
	{
		//iterate through the v values
		for (float v = 0; v <= 1; v += 0.1)
		{
			//the multiplication factor
			constexpr glm::vec3 Multiplier = glm::vec3(1, 5, 1);

			//get the tensor product
			const glm::vec3 TensorProduct = GetTensorProduct(u, v, ControlPoints, UKnots, VKnots);

			//add the vertex
			ReturnVertices.emplace_back(glm::vec3(TensorProduct.x * Multiplier.x, TensorProduct.y * Multiplier.y, TensorProduct.z * Multiplier.z));
		}
	}

	//return the vertices
	return ReturnVertices;
}

std::vector<unsigned int> BSplineTensorProductSurface_::CreateIndices()
{
	//create a vector to store the indices
	std::vector<unsigned int> Indices;

	//reserve space for the indices
	Indices.reserve(160);

	//iterate through the vertices in a grid 10 at a time because we are using a 10x10 grid
	for (int i = 0; i < 90; i += 10)
	{
		//iterate through the vertices in a row
		for (int j = 0; j < 10; j++)
		{
			//add the indices for the first triangle
			Indices.push_back(i + j);
			Indices.push_back(i + j + 1);
			Indices.push_back(i + j + 10);

			//add the indices for the second triangle
			Indices.push_back(i + j + 1);
			Indices.push_back(i + j + 11);
			Indices.push_back(i + j + 10);
		}
	}

	return Indices;
}


