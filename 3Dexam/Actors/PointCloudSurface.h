#pragma once
#include "../Core/WorldObject.h"
#include "../Core/Helpers.h"

//the class id for the point cloud surface
constexpr int PointCloudSurfaceClassID = 1;

//the delimiter used in the file to separate the x, y, and z values
static char PointCloudDelimiter = '	';

//the iterator level for the point cloud surface
static int PointCloudIteratorLevel = 20;

//the file path to the file containing the point cloud data
static std::string PointCloudSurfaceFilePath = "C:/School/2024 2/Visualisering og Simulering/VSim Compulsory 2/3Dexam/Assets/vsim_las.txt";

constexpr int PointCloudGridWidth = 40;
constexpr int PointCloudGridHeight = 125;
constexpr float PointCloudSquareSize = 10.0f;

class PointCloudSurface_ : public WorldObject
{
public:

	//constructor(s)
	PointCloudSurface_(glm::vec3 InPos, glm::vec3 InSize);

	//the 2 extremal corners of the point cloud (in local space, ordered: min, max)
	std::vector<glm::vec3> Corners;

	static std::vector<Vertex> CreateVertices();
	static std::vector<unsigned int> CreateIndices();
	void BeginPlay() override;
};
