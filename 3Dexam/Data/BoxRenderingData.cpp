#include "BoxRenderingData.h"
#include <ostream>

#include "../Systems/WorldSystem.h"

std::vector<Vertex> BoxRenderingData::CreateVertices()
{
	//the colour of the container box
	constexpr glm::vec3 ContainerBoxColour = glm::vec3(0, 1, 0);

	//the friction of the container box
	constexpr float ContainerBoxFriction = 0.5f;

	//storage for the vertices
	std::vector<Vertex> ReturnVertices = {
		// Back face (looking towards -Z)
        {{-0.5, -0.5, -0.5f}, {0,  0, -1}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left
        {{ 0.5,  0.5, -0.5f}, {0,  0, -1}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right
        {{-0.5,  0.5, -0.5f}, {0,  0, -1}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left
        {{ 0.5, -0.5, -0.5f}, {0,  0, -1}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right

        // Front face (looking towards +Z)
        {{-0.5, -0.5,  0.5f}, {0,  0,  1}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left
        {{ 0.5, -0.5,  0.5f}, {0,  0,  1}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right
        {{ 0.5,  0.5,  0.5f}, {0,  0,  1}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right
        {{-0.5,  0.5,  0.5f}, {0,  0,  1}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left

        // Left face (looking towards -X
        {{-0.5, -0.5, -0.5f}, {-1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right
        {{-0.5, -0.5,  0.5f}, {-1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left
        {{-0.5,  0.5,  0.5f}, {-1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left
        {{-0.5,  0.5, -0.5f}, {-1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right

        // Right face (looking towards +X)
        {{ 0.5, -0.5, -0.5f}, {1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left
        {{ 0.5, -0.5,  0.5f}, {1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right
        {{ 0.5,  0.5,  0.5f}, {1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right
        {{ 0.5,  0.5, -0.5f}, {1,  0,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left

        // Top face (looking towards +Y)
        {{-0.5,  0.5, -0.5f}, {0,  1,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left
        {{ 0.5,  0.5, -0.5f}, {0,  1,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right
        {{ 0.5,  0.5,  0.5f}, {0,  1,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right
        {{-0.5,  0.5,  0.5f}, {0,  1,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left

        // Bottom face (looking towards -Y)
        {{-0.5, -0.5, -0.5f}, {0, -1,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 1}}, // Top-left
        {{ 0.5, -0.5, -0.5f}, {0, -1,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 1}}, // Top-right
        {{ 0.5, -0.5,  0.5f}, {0, -1,  0}, ContainerBoxColour, ContainerBoxFriction, {1, 0}}, // Bottom-right
        {{-0.5, -0.5,  0.5f}, {0, -1,  0}, ContainerBoxColour, ContainerBoxFriction, {0, 0}}, // Bottom-left
	};

	//return the vertices
	return ReturnVertices;
}

std::vector<unsigned int> BoxRenderingData::CreateIndices()
{
	//storage for the indices
	std::vector<unsigned int> ReturnIndices = {
        //bottom face
        0, 2, 1, 0, 3, 2,
        //top face
        4, 5, 6, 4, 6, 7,
        //left face
        8, 9, 10, 8, 10, 11,
        //right face
        12, 14, 13, 12, 15, 14,
        //front face
        16, 18, 17, 16, 19, 18,
        //back face
        20, 21, 22, 20, 22, 23
	};

	//return the indices
	return ReturnIndices;
}
