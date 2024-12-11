#include <iostream>

#include "glm/ext/matrix_clip_space.hpp"
#include "Components/CameraComponent.h"
#include "Core/Helpers.h"
#include "Systems/WorldSystem.h"
#include "Core/Shader/Shaders.h"

#include "Components/PlayerControllerComponent.h"
#include "Data/PlaneRenderingData.h"
#include "Entities/BallEntity.h"
#include "Entities/BoxEntity.h"
#include "Entities/StaticEmitterEntity.h"
#include "Entities/StaticEntity.h"

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double XPos, double YPos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

//the game world
WorldSystem GameWorld;

//the shader program
unsigned int ShaderProgram;

//the model key location for the shader program
GLint ModelKeyLocation;

//the current valid class id for a world object
int CurrentValidClassID = 0;

//function to create a glfw window
static GLFWwindow* MyCreateWindow()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create the window
	GLFWwindow* Window = glfwCreateWindow(WindowWidth, WindowHeight, "Exam", nullptr, nullptr);

    //set the window to the current context
	glfwMakeContextCurrent(Window);

    //set callback functions
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetKeyCallback(Window, key_callback);
	glfwSetCursorPosCallback(Window, mouse_callback);
    glfwSetScrollCallback(Window, scroll_callback);
	glfwSetMouseButtonCallback(Window, MouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glad: load all OpenGL function pointers
	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

    //create the shader program
    ShaderProgram = Shaders::CreateShader();

    //use the shader program
    glUseProgram(ShaderProgram);

    //set the model key location
    ModelKeyLocation = glGetUniformLocation(ShaderProgram, ModelKey.c_str());

    return Window;
}

//the window our game will be displayed in
GLFWwindow* GameWindow = MyCreateWindow();

int main()
{
	//seed the random number generator
	srand(time(nullptr));

	//spawn a static entity
	GetWorld()->SpawnObject<StaticEntity>(PlaneRenderingID, glm::vec3(0, -10, 0), glm::vec3(1000));

    //spawn a static emitter entity
	GetWorld()->SpawnObject<StaticEmitterEntity>(glm::vec3(0, 20, 0), glm::vec3(10, 10, 10));

	//spawn a box entity
	const BoxEntity* LocBox = GetWorld()->SpawnObject<BoxEntity>(glm::vec3(150, 70, 150), glm::vec3(100));

     //create extra spheres
	for (int i = 0; i < 500; i++)
	{
        //the spacing for the extra spheres
        const int Spacing = LocBox->TransformComp.Scale.x - 1;

		//create a position for the sphere
		const glm::vec3 SpherePosition = { LocBox->TransformComp.Position.x + rand() % Spacing - Spacing / 2, LocBox->TransformComp.Position.y + rand() % Spacing - Spacing / 2, LocBox->TransformComp.Position.z + rand() % Spacing - Spacing / 2 };

		//create a random velocity for the sphere
        const glm::vec3 SphereVelocity = { RandomFloat(-10, 10), RandomFloat(-10, 10), RandomFloat(-10, 10) };

		//spawn the ball entity
		GetWorld()->SpawnObject<BallEntity>(SpherePosition, glm::vec3(1), SphereVelocity);
	}


	//set the position of the light source
	glm::vec3 LightPos = GameWorld.GetPlayerCamera()->Position;

    //set the light source position
	glUniform3fv(glGetUniformLocation(ShaderProgram, LightPosKey.c_str()), 1, &LightPos[0]);

    //set the light on flag to true
	glUniform1i(glGetUniformLocation(ShaderProgram, LightFlagKey.c_str()), 1);

    //initialize the world
	GetWorld()->InitializeWorld();

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    // timing (from https://learnopengl.com/Getting-started/Camera)
	// time between current frame and last frame
	float lastFrame = 0;

    // render loop
    while (!glfwWindowShouldClose(GameWindow))
    {
        // per-frame time logic (from https://learnopengl.com/Getting-started/Camera)
        // --------------------
        const float currentFrame = glfwGetTime();
        const float DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //draw the background
        glClearColor(0.2f, 0.3f, 0.3f, 1);

        //clear the colour buffer
		glClear(GL_COLOR_BUFFER_BIT);

        //clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        //glBindVertexArray(VAO);
        glUseProgram(ShaderProgram);

        //(modified)from https://learnopengl.com/Getting-started/Camera
        glm::mat4 projection = glm::perspective(glm::radians(GameWorld.GetPlayerCamera()->Zoom), WindowWidth / WindowHeight, 0.1f, CullingDistance);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, ProjectionKey.c_str()), 1, false, &projection[0][0]);

        // camera/view transformation
        glm::mat4 View = GameWorld.GetPlayerCamera()->GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram,ViewKey.c_str()), 1, false, &View[0][0]);

    	//model matrix
        glm::mat4 model = glm::mat4(1);

        //set the model matrix uniform
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, ModelKey.c_str()), 1, false, &model[0][0]);

        //do the tick and rendering for this frame
        GameWorld.UpdateWorld(DeltaTime);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(GameWindow);
        glfwPollEvents();
    }

    //// optional: de-allocate all resources once they've outlived their purpose:
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(SHADER_REF);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

//called whenever the window is resized
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    std::cout << " windows resized with " << width << " Height " << height << '\n';
}

//called whenever the mouse is moved
void mouse_callback(GLFWwindow* window, const double XPos, const double YPos)
{
	GetWorld()->GetPlayerController()->ProcessMouseInput(float(XPos), float(YPos));
}

//called whenever a key is pressed
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetWorld()->GetPlayerController()->ProcessKeyboardInput(window, key, scancode, action, mods);
}

//called whenever the mouse scroll wheel is moved
void scroll_callback(GLFWwindow* window, double xoffset, const double yoffset)
{
    GetWorld()->GetPlayerController()->ProcessMouseScroll(float(yoffset));
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//get the camera of the player
	CameraComponent* PlayerCamera = GetWorld()->GetPlayerCamera();

	//check if the left mouse button was pressed
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
        //get the position of the camera
		glm::vec3 CameraPos = PlayerCamera->Position;

		//get the front vector of the camera
		glm::vec3 FrontVec = PlayerCamera->Front;

		//get the vertices of the point cloud
        const std::vector<Vertex> TestVertices = GetWorld()->GetRenderingSystem()->GetWorldVertices(GetWorld()->GetEntityOfClass<StaticEntity>());

		//iterate through the vertices in sets of 3
		for (int i = 0; i < TestVertices.size(); i += 3)
		{
   
			//get the current triangle
			const Vertex& P1 = TestVertices[i % 3];
			const Vertex& P2 = TestVertices[(i + 1) % 3];
			const Vertex& P3 = TestVertices[(i + 2) % 3];

			//storage for the collision point
			Vertex CollisionPoint = Vertex(glm::vec3(0, 0, 0));

			//do a collision check
			if (testRayThruTriangle(P1, P2, P3, Vertex(CameraPos), Vertex(CameraPos + FrontVec * CullingDistance), CollisionPoint))
			{
				//spawn a sphere at the collision point
				BallEntity* NewSphere = GetWorld()->SpawnObject<BallEntity>(CollisionPoint.Position + glm::vec3(0, 1, 0) , glm::vec3(5));

				////get the front vector of the camera projected onto the normal of the triangle
				//glm::vec3 ProjectedFront = normalize(FrontVec - CollisionPoint.Normal * dot(FrontVec, CollisionPoint.Normal));

				////set the velocity of the sphere
				//NewSphere->Velocity = ProjectedFront * 50.f;

				//break out of the loop
				break;
			}
		}
	}

}