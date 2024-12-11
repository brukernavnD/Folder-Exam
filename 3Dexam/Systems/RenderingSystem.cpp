#include "RenderingSystem.h"
#include "glm/gtx/transform.hpp"
#include "../Entities/Entity.h"
#include "../Components/MeshComponent.h"
#include "../Components/TransformComponent.h"

void RenderingSystem::AddBuffers(const std::vector<Vertex>& InVertices, const std::vector<unsigned int>& InIndices)
{
	//storage for the vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//storage for the vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, InVertices.size() * sizeof(Vertex), InVertices.data(), GL_STATIC_DRAW);

	//storage for the element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, InIndices.size() * sizeof(unsigned int), InIndices.data(), GL_STATIC_DRAW);

	//position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));

	//color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));

    //normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));

    //texture attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

	//add the vaos, vbos, and ebos to the storage
	VAOs.push_back(VAO);
	VBOs.push_back(VBO);
	EBOs.push_back(EBO);

	//add the vertices and indices to the storage
	Vertices.push_back(InVertices);
	Indices.push_back(InIndices);

	//add the polygon mode to the storage
	PolygonModes.push_back(GL_FILL);

	//create a new vector to store the vertex pointers
	std::vector<Vertex*> LocVertexPointers;

	//loop through the vertices and add the pointers to the vector
	for (Vertex& LocVertex : Vertices.back())
	{
		LocVertexPointers.push_back(&LocVertex);
	}

	//add the vertex pointers to the storage
	VertexPointers.push_back(LocVertexPointers);
}

int RenderingSystem::GetNewValidRenderingID(const bool Increment)
{
	if (Increment)
	{
		return CurrentValidRenderingID++;
	}

	return CurrentValidRenderingID;
}

std::vector<Vertex*> RenderingSystem::GetVertexPointers(const int RenderingID) const
{
	return VertexPointers.at(RenderingID);
}

void RenderingSystem::RenderWorldObjects(std::vector<Entity*> InWorldObjects) const
{
	//the current class index used for picking the correct vertices
	int RenderingID = -1;

	//loop through the world objects
	for (Entity* WorldObject : InWorldObjects)
	{
		//storage for the mesh component of the object
		const MeshComponent* MeshComp = WorldObject->GetComponent<MeshComponent>();

		//check if the entity does not have a mesh component
		if (!MeshComp)
		{
			//continue to the next object
			continue;
		}

		//storage for the transform component of the object
		const TransformComponent* TransformComp = WorldObject->GetComponent<TransformComponent>();

		//check if the entity does not have a transform component
		if (!TransformComp)
		{
			//continue to the next object
			continue;
		}

		//check if the class id is different
		if (MeshComp->RenderingID != RenderingID)
		{
			//check if the current class index is valid
			if (RenderingID != -1)
			{
				//unbind the previous vertex array object
				glBindVertexArray(0);
			}

			//set the rendering id
			RenderingID = MeshComp->RenderingID;

			//bind the vertex array object
			glBindVertexArray(VAOs[RenderingID]);

			//set the current polygon mode
			glPolygonMode(GL_FRONT_AND_BACK, PolygonModes.at(RenderingID));
		}

		//get the model matrix of the object
		const glm::mat4 Model = TransformComp->GetTransformMatrix();

		//set the model matrix
		glUniformMatrix4fv(ModelKeyLocation, 1, GL_FALSE, &Model[0][0]);

		//draw the world object
		glDrawElements(GL_TRIANGLES, Indices.at(RenderingID).size(), GL_UNSIGNED_INT, nullptr);
	}
}

std::vector<Vertex> RenderingSystem::GetWorldVertices(Entity* InObject) const
{
	//storage for the transform component of the object
	const TransformComponent* TransformComp = InObject->GetComponent<TransformComponent>();

	//check if the entity does not have a transform component
	if (!TransformComp)
	{
		//return an empty vector
		return {};
	}

	//storage for the mesh component of the object
	const MeshComponent* MeshComp = InObject->GetComponent<MeshComponent>();

	//check if the entity does not have a mesh component
	if (!MeshComp)
	{
		//return an empty vector
		return {};
	}

	//get the model matrix of the object
	const glm::mat4 Model = TransformComp->GetTransformMatrix();

	//create a vector to store the world vertices
	std::vector<Vertex> WorldVertices;
	WorldVertices.reserve(Vertices.at(MeshComp->RenderingID).size());

	//loop through the vertices and add the world position to the world vertices
	for (Vertex LocVertex : Vertices.at(MeshComp->RenderingID))
	{
		//get the world position
		const glm::vec3 WorldPosition = Model * glm::vec4(LocVertex.Position, 1);

		//create a new vertex with the world position
		Vertex NewVertex = LocVertex;
		NewVertex.Position = WorldPosition;

		//add the world position to the world vertices
		WorldVertices.emplace_back(NewVertex);
	}

	return WorldVertices;
}

GLenum RenderingSystem::GetPolygonMode(const int RenderingID) const
{
	return PolygonModes.at(RenderingID);
}

void RenderingSystem::SetPolygonMode(const int RenderingID, const GLenum InPolygonMode)
{
	PolygonModes.at(RenderingID) = InPolygonMode;
}
