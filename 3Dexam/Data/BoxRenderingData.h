#pragma once

#include "../Core/Helpers.h"

constexpr int BoxRenderingID = 1;

class BoxRenderingData
{
public:

	//functions to create the vertices and indices for a box
	static std::vector<Vertex> CreateVertices();
	static std::vector<unsigned int> CreateIndices();
};
