#include "Vertex.h"

Vertex::Vertex()
{
	Position = glm::vec3(0);
	Normal = glm::vec3(-1);
	Color = glm::vec3(0, 0, 1);
}

Vertex::Vertex(const glm::vec3 InPos)
{
	Position = InPos;
	Normal = glm::vec3(1);
	Color = glm::vec3(0, 0, 1);
	TexCoords = glm::vec2(0);
}

Vertex::Vertex(glm::vec3 InPos, glm::vec3 InNormal, glm::vec3 InColor, float InFriction, glm::vec2 InTextureCoordinates)
{
	Position = InPos;
	Normal = InNormal;
	Color = InColor;
	Friction = InFriction;
	TexCoords = InTextureCoordinates;
}

bool Vertex::operator==(const Vertex& InVertex) const
{
	return Position == InVertex.Position && Normal == InVertex.Normal && Color == InVertex.Color && TexCoords == InVertex.TexCoords;
}
