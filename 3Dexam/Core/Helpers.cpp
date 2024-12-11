#include "Helpers.h"
#include <iostream>
#include <random>

#include "Model/Vertex.h"
#include "../Systems/WorldSystem.h"
#include "../Entities/Entity.h"

WorldSystem* GetWorld()
{
	return &GameWorld;
}

GLenum glCheckError_(const Entity* InWorldObject, const char* function, const int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			//0x0500 is the value of GL_INVALID_FRAMEBUFFER_OPERATION
			case 0x0506: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		//check if the world object is valid
		if (InWorldObject != nullptr)
		{
			std::cout << InWorldObject->Name << " | " << error << " | " << function << " (" << line << ")" << '\n';
		}
		else
		{
			std::cout << error << " | " << function << " (" << line << ")" << '\n';
		}
	}
	return errorCode;
}

int IDCounter()
{
	return GameWorld.CurrentValidID++;
}

int ClassIDCounter()
{
	return CurrentValidClassID++;
}

int RenderDataIDCounter()
{
	//return the current valid rendering id
	return GameWorld.GetRenderingSystem()->GetNewValidRenderingID();
}

int RandomInt(const int min, const int max)
{
	static bool init = false;
	static std::random_device rd;
	static std::mt19937 eng;
	static std::uniform_int_distribution<int> dist(min, max);
	if (!init) {
		eng.seed(rd()); // Seed random engine
		init = true;
	}
	return dist(eng);
}

float RandomFloat(const float Min, const float Max)
{
	return Min + static_cast <float> (rand()) / (RAND_MAX / (Max - Min));
}

//from https://gamedev.stackexchange.com/questions/5585/line-triangle-intersection-last-bits
bool testRayThruTriangle(Vertex P1, Vertex P2, Vertex P3, Vertex R1, Vertex R2, Vertex& PIP)
{
	glm::vec3 Normal, IntersectPos;

	// Find Triangle Normal
	Normal = normalize(cross(P2.Position - P1.Position, P3.Position - P1.Position));

	// Find distance from LP1 and LP2 to the plane defined by the triangle
	float Dist1 = dot(R1.Position-P1.Position, Normal);
	float Dist2 = dot(R2.Position-P1.Position, Normal);

	if (Dist1 * Dist2 >= 0.0f)
	{
		return false;
	} // line doesn't cross the triangle.

	if ( Dist1 == Dist2)
	{
		return false;
	} // line and plane are parallel

	// Find point on the line that intersects with the plane
	IntersectPos = R1.Position + (R2.Position-R1.Position) * (-Dist1/(Dist2-Dist1));

	// Find if the interesection point lies inside the triangle by testing it against all edges
	glm::vec3 vTest;

	vTest = cross(Normal, P2.Position-P1.Position);
	if (dot(vTest, IntersectPos-P1.Position) < 0.0f)
	{
		return false;
	}

	vTest = cross(Normal, P3.Position-P2.Position);
	if (dot(vTest, IntersectPos-P2.Position) < 0.0f)
	{
		return false;
	}

	vTest = cross(Normal, P1.Position-P3.Position);
	if (dot(vTest, IntersectPos-P1.Position) < 0.0f)
	{
		return false;
	}

	PIP = Vertex(IntersectPos, Normal);
	return true;
}

glm::vec3 GetBaryCentricCoord(const glm::vec3& Position, Vertex A, Vertex B, Vertex C)
{
	//get the determinant
	float det = (B.Position.z - C.Position.z) * (A.Position.x - C.Position.x) + (C.Position.x - B.Position.x) * (A.Position.z - C.Position.z);

	//avoid division by zero
	if(det == 0)
	{
		det=0.0001f;
	}

	//get the first barycentric coordinate
	float b1 = (B.Position.z - C.Position.z) * (Position.x - C.Position.x) + (C.Position.x - B.Position.x) * (Position.z - C.Position.z) / det;

	//get the second barycentric coordinate
	float b2 = (C.Position.z - A.Position.z) * (Position.x - C.Position.x) + (A.Position.x - C.Position.x) * (Position.z - C.Position.z) / det;

	//get the last barycentric coordinate by subtracting the first two from 1
	float b3 = 1 - b1 - b2;

	return {b1, b2, b3};
}

glm::vec3 RandomVec3(const glm::vec3 Center, const float Offset)
{
	return { RandomFloat(Center.x - Offset, Center.x + Offset), RandomFloat(Center.y - Offset, Center.y + Offset), RandomFloat(Center.z - Offset, Center.z + Offset) };
}

unsigned CreateTexture(const glm::vec4& Colour)
{
	//create the texture
	unsigned int Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//create the texture
	const unsigned char Data[4] = { static_cast<unsigned char>(Colour.r * 255), static_cast<unsigned char>(Colour.g * 255), static_cast<unsigned char>(Colour.b * 255), static_cast<unsigned char>(Colour.a * 255) };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//return the texture
	return Texture;
}