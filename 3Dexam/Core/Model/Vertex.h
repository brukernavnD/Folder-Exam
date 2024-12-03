#pragma once

#include <glm/vec3.hpp>
#include "glm/vec2.hpp"

//based off of the vertex struct from learnopengl.com
struct Vertex
{
	//our position
	glm::vec3 Position = glm::vec3(0);

	//our color
	glm::vec3 Color = glm::vec3(0);

	//our normal
	glm::vec3 Normal = glm::vec3(0);

	//our texture coordinates
	glm::vec2 TexCoords = glm::vec2(0);

	//our friction value
	float Friction = 0;

	//constructor(s)
	Vertex();
	explicit Vertex(glm::vec3 InPos);
	Vertex(glm::vec3 InPos, glm::vec3 InNormal, glm::vec3 InColor = glm::vec3(0), float InFriction = 0, glm::vec2 InTextureCoordinates = glm::vec2(0));

	//operator overloads
	bool operator==(const Vertex& InVertex) const;
};

