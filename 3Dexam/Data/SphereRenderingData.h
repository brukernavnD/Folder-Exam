#pragma once

#include "../Core/Helpers.h"

constexpr int SphereRenderingID = 0;

//much of the code for generating the vertices and indicies of this object was adapted from https://www.songho.ca/opengl/gl_sphere.html
class SphereRenderingData
{
public:

	//functions to create the vertices and indices for a sphere
	static std::vector<Vertex> CreateVertices();
	static std::vector<unsigned int> CreateIndices();


};