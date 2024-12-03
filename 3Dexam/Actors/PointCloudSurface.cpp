#include "PointCloudSurface.h"

#include <fstream>
#include <iostream>
#include <ostream>


#include "../Core/World.h"

PointCloudSurface_::PointCloudSurface_(glm::vec3 InPos, glm::vec3 InSize) : WorldObject(InPos, InSize, {0,0,0}, "PointCloudSurface (" + std::to_string(GameWorld.CurrentValidID + 1) + ")")
{
	ClassID = PointCloudSurfaceClassID;
}


// Helper function to calculate the average height (y-coordinate) of points within a given square
float calculateAverageHeight(const std::vector<Vertex>& points, float xMin, float xMax, float zMin, float zMax) {
    float totalHeight = 0.0f;
    int count = 0;

    for (const auto& point : points) {
        // Check if the point lies within the current square bounds in the x-z plane
        if (point.Position.x >= xMin && point.Position.x < xMax && point.Position.z >= zMin && point.Position.z < zMax) {
            totalHeight += point.Position.y; // y is the height
            count++;
        }
    }

    // Return average height, or 0 if no points were in this square
    return count > 0 ? totalHeight / count : 0.0f;
}

std::vector<Vertex> createContinuousHeightMappedGrid(int gridWidth, int gridHeight, float squareSize, const std::vector<Vertex>& points) {
    // Create a 2D grid of vertices with computed heights
    std::vector<std::vector<Vertex>> gridVertices(gridWidth + 1, std::vector<Vertex>(gridHeight + 1));

    // Fill in the heights for each vertex in the grid
    for (int z = 0; z <= gridHeight; ++z) {
        for (int x = 0; x <= gridWidth; ++x) {
            // Calculate the bounds of the square that this vertex is part of
            float xMin = x * squareSize;
            float xMax = xMin + squareSize;
            float zMin = z * squareSize;
            float zMax = zMin + squareSize;

            // Calculate the average height for the points within this square
            float averageHeight = calculateAverageHeight(points, xMin, xMax, zMin, zMax);

            // Store the vertex position in the grid, with y representing the height
            gridVertices[x][z] = Vertex(glm::vec3(xMin, averageHeight, zMin));
        }
    }

    // Generate the triangle vertices using the grid, ensuring continuity
    std::vector<Vertex> vertices;
    vertices.reserve(gridWidth * gridHeight * 6); // 6 vertices per square (2 triangles)

    for (int z = 0; z < gridHeight; ++z) {
        for (int x = 0; x < gridWidth; ++x) {

			//the colour of the point cloud
			glm::vec3 PointCloudColour = glm::vec3(0, .5, 0);

			//the surface friction of the point cloud
			float PointCloudFriction = 0.5;

			//check if we're halfway or more through the point cloud's height
			if (z >= gridHeight / 2)
			{
				//set the friction to 0.1
				PointCloudFriction = 0.1f;

				//set the colour to red
				PointCloudColour = glm::vec3(.5, 0, 0);
			}


	        // Get the four corners of the current square from the precomputed grid vertices
            Vertex bottomLeft = gridVertices[x][z];
            Vertex bottomRight = gridVertices[x + 1][z];
            Vertex topLeft = gridVertices[x][z + 1];
            Vertex topRight = gridVertices[x + 1][z + 1];

			//calculate the normals for the vertices of the first triangle
			glm::vec3 normal1 = normalize(cross(topLeft.Position - bottomLeft.Position, topRight.Position - bottomLeft.Position));
			glm::vec3 normal2 = normalize(cross(topRight.Position - bottomRight.Position, topLeft.Position - bottomRight.Position));
			glm::vec3 normal3 = normalize(cross(bottomRight.Position - bottomLeft.Position, topRight.Position - bottomLeft.Position));

            // First triangle (bottom left, top left, top right)
            vertices.emplace_back(bottomLeft.Position, normal1, PointCloudColour, PointCloudFriction);
			vertices.emplace_back(topLeft.Position, normal2, PointCloudColour, PointCloudFriction);
			vertices.emplace_back(topRight.Position, normal3, PointCloudColour, PointCloudFriction);

			//calculate the normals for the vertices of the second triangle
			glm::vec3 normal4 = normalize(cross(bottomLeft.Position - bottomRight.Position, topRight.Position - bottomRight.Position));
			glm::vec3 normal5 = normalize(cross(topRight.Position - topLeft.Position, bottomLeft.Position - topLeft.Position));
			glm::vec3 normal6 = normalize(cross(bottomRight.Position - bottomLeft.Position, topRight.Position - bottomLeft.Position));

            // Second triangle (bottom left, top right, bottom right)
			vertices.emplace_back(bottomLeft.Position, normal4, PointCloudColour, PointCloudFriction);
			vertices.emplace_back(topRight.Position, normal5, PointCloudColour, PointCloudFriction);
			vertices.emplace_back(bottomRight.Position, normal6, PointCloudColour, PointCloudFriction);
        }
    }

    return vertices;
}

std::vector<Vertex> PointCloudSurface_::CreateVertices()
{
	//open our point cloud file
	std::ifstream FileReader(PointCloudSurfaceFilePath);

	// Check if the file is successfully opened
    if (!FileReader.is_open()) {
	    std::cerr << "Error opening the file!";
		return {};
    }

	//storage for each line of the file
	std::string LineString;

	//storage for the vertices
	std::vector<Vertex> TempVertices;

	//storage for the line number we're on
	int LineNumber = 0;

	//iterate through each line of the file
    while (getline(FileReader, LineString))
    {
		//check if this is the first line
		if (LineNumber == 0)
		{
			//increment the line number early
			LineNumber += PointCloudIteratorLevel;

			//convert the first line to an integer
			int NumVertices = std::stoi(LineString);

			//reserve space for the vertices
			TempVertices.reserve(NumVertices);

			//skip further processing of this line
			continue;
		}

		//storage for the part of the line we're on
		std::vector<std::string> LineParts;

		//add the first part of the line
		LineParts.emplace_back("");
		
		//iterate through each character in the line
		for (const char CurrentChar : LineString)
		{
			//check if the character is the delimiter
			if (CurrentChar == PointCloudDelimiter)
			{
				//add a new part to the line
				LineParts.emplace_back("");
			}
			else
			{
				//add the character to the current part
				LineParts[std::max(int(LineParts.size() - 1), 0)] += CurrentChar;
			}
		}

		//create the new vertex
		Vertex NewVertex = Vertex(glm::vec3(std::stof(LineParts[0]), std::stof(LineParts[2]), std::stof(LineParts[1])));

		//add the new vertex to the vertices
		TempVertices.emplace_back(NewVertex);

		//increment the line number
		LineNumber++;
    }

	//storage for the point closest to the origin
	glm::vec3 ClosestPoint = glm::vec3(FLT_MAX);

	//iterate through the vertices and find the closest point to the origin
	for (const Vertex& CurrentVertex : TempVertices)
	{
		//check if the current vertex is closer to the origin than the current closest point
		if (length(CurrentVertex.Position) < length(ClosestPoint))
		{
			//set the closest point to the current vertex
			ClosestPoint = CurrentVertex.Position;
		}
	}

	//iterate through the vertices and move them so the closest point is at the origin
	for (Vertex& CurrentVertex : TempVertices)
	{
		//move the vertex
		CurrentVertex.Position -= ClosestPoint;
	}
 
    // Close the file
    FileReader.close();

	//convert the vertices to glm::vec3 vector
	std::vector<glm::vec3> points;
	points.reserve(TempVertices.size());

	for (const Vertex& vertex : TempVertices) {
		points.push_back(vertex.Position);
	}

	// Create a grid of vertices with height mapped from the point cloud
	const std::vector<Vertex> gridVertices = createContinuousHeightMappedGrid(PointCloudGridWidth, PointCloudGridHeight, PointCloudSquareSize, TempVertices);

	//return the vertices
    return gridVertices;
}

std::vector<unsigned int> PointCloudSurface_::CreateIndices()
{
	//get the vertices
	const std::vector<Vertex> Vertices = CreateVertices();

	//create a vector to store the indices
	std::vector<unsigned int> Indices;
	Indices.reserve(Vertices.size());

	//loop through the vertices and add the index to the indices
	for (unsigned int Index = 0; Index < Vertices.size(); Index++)
	{
		//add the index to the indices
		Indices.emplace_back(Index);
	}

	return Indices;
}

void PointCloudSurface_::BeginPlay()
{
	//call the parent implementation
	WorldObject::BeginPlay();

	//get our vertices from the render helper
	const std::vector<Vertex*> Vertices = GetWorld()->Renderer.GetVertexPointers(PointCloudSurfaceClassID);

	//add the 2 extremal corners of the point cloud
	Corners = { glm::vec3(FLT_MAX, 0, FLT_MAX), glm::vec3(-FLT_MAX, 0, -FLT_MAX) };

	//iterate through the vertices
	for (Vertex* LocVertex : Vertices)
	{
		//check if the x value is less than the minimum x value
		if (LocVertex->Position.x < Corners.at(0).x)
		{
			//set the minimum x value
			Corners.at(0).x = LocVertex->Position.x;
		}

		//check if the x value is greater than the maximum x value
		if (LocVertex->Position.x > Corners.at(1).x)
		{
			//set the maximum x value
			Corners.at(1).x = LocVertex->Position.x;
		}

		//check if the z value is less than the minimum z value
		if (LocVertex->Position.z < Corners.at(0).z)
		{
			//set the minimum z value
			Corners.at(0).z = LocVertex->Position.z;
		}

		//check if the z value is greater than the maximum z value
		if (LocVertex->Position.z > Corners.at(1).z)
		{
			//set the maximum z value
			Corners.at(1).z = LocVertex->Position.z;
		}
	}
}


