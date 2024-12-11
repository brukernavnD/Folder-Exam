#pragma once
#include <vector>
#include <string>
#include "glm/glm.hpp"

//include opengl and glfw for the rest of the project
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//forward declaration(s)
class Entity;
class WorldSystem;
struct Vertex;

//storage variables for inputs to the shaders
const std::string ProjectionKey = "Projection";
const std::string ViewKey = "View";
const std::string ModelKey = "Model";
const std::string LightFlagKey = "LightOn";
const std::string LightPosKey = "LightPos";

//get the model key location in the shader program
extern GLint ModelKeyLocation;

//the culling distance used when rendering objects
static float CullingDistance = 10000000.0f;

//the window's width and height
static float WindowWidth = 2048;
static float WindowHeight = 1080;

//the current shader program
extern unsigned int ShaderProgram;

//function to get a pointer to the game world
WorldSystem* GetWorld();

//the current game world
extern WorldSystem GameWorld;

//the window the game is running in
extern GLFWwindow* GameWindow;

//the current valid id for a world object class
extern int CurrentValidClassID;

//opengl error checking code from https://learnopengl.com/In-Practice/Debugging (modified)
GLenum glCheckError_(const Entity* InWorldObject, const char* function, const int line);

//function to get the next valid id for a world object
int IDCounter();

//function to get the next valid class id for an entity class
int ClassIDCounter();

//function to get the next valid render id for rendering data and add the vertices and indices to the rendering system
int RenderDataIDCounter();

//function to get a random number between two values taken from https://stackoverflow.com/questions/53040940/why-is-the-new-random-library-better-than-stdrand
int RandomInt(int min, int max);

//function to get a random float between two values
float RandomFloat(float Min, float Max);

//function to get a position around a center point with a random offset
glm::vec3 RandomVec3(glm::vec3 Center, float Offset);

//function to create a texture for opengl using a single rgba value
unsigned int CreateTexture(const glm::vec4& Colour);

//function to test if a ray intersects a triangle (from https://gamedev.stackexchange.com/questions/5585/line-triangle-intersection-last-bits)
bool testRayThruTriangle(Vertex P1, Vertex P2, Vertex P3, Vertex R1, Vertex R2, Vertex& PIP);

//define for the error checking outside of world objects
#define glCheckError() glCheckError_(nullptr, __FUNCTION__, __LINE__)

//define for the error checking inside of world objects
#define glWCheckError() glCheckError_(this, __FUNCTION__, __LINE__)