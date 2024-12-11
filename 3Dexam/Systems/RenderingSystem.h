#pragma once
#include "../Core/Helpers.h"
#include "../Core/Model/Vertex.h"

class RenderingSystem
{
	//storage for unique vertex array buffer objects
	std::vector<unsigned int> VAOs;

	//storage for unique vertex buffer objects
	std::vector<unsigned int> VBOs;

	//storage for unique element buffer objects
	std::vector<unsigned int> EBOs;

	//storage for the vertices for different types of objects (using the class id of the object)
	std::vector<std::vector<Vertex>> Vertices;

	//storage for the pointers to the vertices
	std::vector<std::vector<Vertex*>> VertexPointers;

	//storage for the indices for different types of objects (using the class id of the object)
	std::vector<std::vector<unsigned int>> Indices;

	//storage for the polygon mode for different types of objects (using the class id of the object)
	std::vector<GLenum> PolygonModes;

	//the current valid rendering id
	int CurrentValidRenderingID = 0;

public:

	//function to add the vaos, vbos, and ebos to the for a world object
	void AddBuffers(const std::vector<Vertex>& InVertices, const std::vector<unsigned int>& InIndices);

	//function to get the current valid buffer index
	int GetNewValidRenderingID(bool Increment = true);

	//function to get the vertex pointers for a class id
	std::vector<Vertex*> GetVertexPointers(int RenderingID) const;

	//rendering function for the world objects
	void RenderWorldObjects(std::vector<Entity*> InWorldObjects) const;

	//function to get the world vertices of a world object
	std::vector<Vertex> GetWorldVertices(Entity* InObject) const;

	//function to get polygon mode for a rendering id
	GLenum GetPolygonMode(int RenderingID) const;

	//function to set the polygon mode for a rendering id
	void SetPolygonMode(int RenderingID, GLenum InPolygonMode);
};

