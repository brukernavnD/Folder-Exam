#include "PlaneRenderingData.h"
#include "../Core/Model/Vertex.h"

std::vector<Vertex> PlaneRenderingData::CreateVertices()
{
	//storage for the vertices
	std::vector<Vertex> Vertices;

	//reserve the space for the vertices
	Vertices.reserve(4);

	//create the vertices
	Vertex Vertex;
	Vertex.Position = glm::vec3(-1.0f, 0.0f, -1.0f);
	Vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertex.TexCoords = glm::vec2(0.0f, 0.0f);
	Vertices.push_back(Vertex);

	Vertex.Position = glm::vec3(-1.0f, 0.0f, 1.0f);
	Vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertex.TexCoords = glm::vec2(0.0f, 1.0f);
	Vertices.push_back(Vertex);

	Vertex.Position = glm::vec3(1.0f, 0.0f, 1.0f);
	Vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertex.TexCoords = glm::vec2(1.0f, 1.0f);
	Vertices.push_back(Vertex);

	Vertex.Position = glm::vec3(1.0f, 0.0f, -1.0f);
	Vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	Vertex.TexCoords = glm::vec2(1.0f, 0.0f);
	Vertices.push_back(Vertex);

	//return the vertices
	return Vertices;
}

std::vector<unsigned int> PlaneRenderingData::CreateIndices()
{
	//storage for the indices
	std::vector<unsigned int> Indices;

	//reserve the space for the indices
	Indices.reserve(6);

	//create the indices
	Indices.push_back(0);
	Indices.push_back(1);
	Indices.push_back(2);
	Indices.push_back(0);
	Indices.push_back(2);
	Indices.push_back(3);

	//return the indices
	return Indices;
}
