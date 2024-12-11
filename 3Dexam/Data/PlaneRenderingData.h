#pragma once
#include "../Core/Helpers.h"

constexpr int PlaneRenderingID = 2;

class PlaneRenderingData
{
public:

	//functions to create the vertices and indices
	static std::vector<Vertex> CreateVertices();
	static std::vector<unsigned int> CreateIndices();


};

