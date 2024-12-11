#include "SphereRenderingData.h"

#include "../Systems/WorldSystem.h"

constexpr int Segments = 16;
constexpr float PI = 3.14159265359f;

std::vector<Vertex> SphereRenderingData::CreateVertices()
{
	//storage for the vertices
	std::vector<Vertex> Vertices;

	//reserve the space for the vertices
	Vertices.reserve((Segments + 1) * (Segments + 1));

	for (int y = 0; y <= Segments; ++y)
	{
		for (int x = 0; x <= Segments; ++x)
		{
			const float xSegment = static_cast<float>(x) / static_cast<float>(Segments);
			const float ySegment = static_cast<float>(y) / static_cast<float>(Segments);
			const float xPos = std::cos(xSegment * 2 * PI) * std::sin(ySegment * PI);
			const float yPos = std::cos(ySegment * PI);
			const float zPos = std::sin(xSegment * 2 * PI) * std::sin(ySegment * PI);

			Vertex Vertex;
			Vertex.Position = glm::vec3(xPos, yPos, zPos);
			Vertex.Normal = glm::vec3(xPos, yPos, zPos);
			Vertex.Color = glm::vec3(1);
			Vertex.TexCoords = glm::vec2(xSegment, ySegment);
			Vertices.push_back(Vertex);
		}
	}

	return Vertices;
}

std::vector<unsigned int> SphereRenderingData::CreateIndices()
{
	//storage for the indices
	std::vector<unsigned int> Indices;

	//reserve the space for the indices
	Indices.reserve(Segments * Segments * 6);

	for (int y = 0; y < Segments; ++y)
	{
		for (int x = 0; x < Segments; ++x)
		{
			Indices.push_back(y * (Segments + 1) + x);
			Indices.push_back((y + 1) * (Segments + 1) + x);
			Indices.push_back(y * (Segments + 1) + x + 1);

			Indices.push_back(y * (Segments + 1) + x + 1);
			Indices.push_back((y + 1) * (Segments + 1) + x);
			Indices.push_back((y + 1) * (Segments + 1) + x + 1);
		}
	}

	return Indices;
}
