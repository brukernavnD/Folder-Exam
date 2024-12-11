#include "PlayerControllerComponent.h"

#include <map>
#include <ostream>
#include "../Core/Helpers.h"
#include "../Data/BoxRenderingData.h"
#include "../Systems/WorldSystem.h"

PlayerInput_::PlayerInput_() = default;

EInputState PlayerInput_::GetKeyState(const int Key)
{
	//get the current key state
	const EInputState CurrentKeyState = glfwGetKey(GameWindow, Key) == GLFW_PRESS ? EInputState::Pressed : EInputState::NotPressed;

	//check if the key is not in the keys to check vector
	if (std::count(KeysToCheck.begin(), KeysToCheck.end(), std::pair<int, EInputState>(Key, EInputState::Pressed)) == 0)
	{
		//check if the key is currently pressed
		if (CurrentKeyState == EInputState::Pressed)
		{
			//add the key to the keys to check vector
			KeysToCheck.emplace_back(Key, EInputState::Pressed);
		}

		//return the current key state
		return CurrentKeyState;
	}

	//get the index of the key in the keys to check vector
	const int KeyIndex = std::distance(KeysToCheck.begin(), std::find(KeysToCheck.begin(), KeysToCheck.end(), std::pair<int, EInputState>(Key, EInputState::Pressed)));

	//check if the key was pressed last frame and isn't currently pressed
	if (KeysToCheck.at(KeyIndex).second == EInputState::Pressed && CurrentKeyState == EInputState::NotPressed)
	{
		//set the key state to released
		KeysToCheck.at(KeyIndex) = std::pair<int, EInputState>(Key, EInputState::Released);

		//return the released key state
		return EInputState::Released;
	}

	//return the current key state
	return CurrentKeyState;
}

bool PlayerInput_::IsPressed(const int Key)
{
	//return the key state
	return GetKeyState(Key) == EInputState::Pressed;
}

bool PlayerInput_::WasReleased(const int Key)
{
	//return the key state
	return GetKeyState(Key) == EInputState::Released;
}

void PlayerInput_::ResetKeysData()
{
	//iterate through the keys to check vector
	for (std::pair<int, EInputState>& Key : KeysToCheck)
	{
		//check if the key was released last frame
		if (Key.second == EInputState::Released)
		{
			//remove the key from the keys to check vector
			KeysToCheck.erase(std::remove(KeysToCheck.begin(), KeysToCheck.end(), Key), KeysToCheck.end());
		}
	}
}

PlayerControllerComponent::PlayerControllerComponent() = default;

void PlayerControllerComponent::ProcessKeyboardInput(GLFWwindow* Window, int Key, int ScanCode, int Action, int Mods)
{
	//check if the escape key is pressed or held
    if (Key == GLFW_KEY_ESCAPE && (Action == GLFW_PRESS || Action == GLFW_REPEAT)) 
	{
		glfwSetWindowShouldClose(Window, true);
	}


	//check if the 1 key was just released
	if (PlayerInput.WasReleased(GLFW_KEY_1))
	{
		//check if the polygon mode is GL_FILL
		if (GetWorld()->GetRenderingSystem()->GetPolygonMode(BoxRenderingID) == GL_FILL)
		{
			//set the polygon mode to GL_LINE
			GetWorld()->GetRenderingSystem()->SetPolygonMode(BoxRenderingID, GL_LINE);
		}
		else
		{
			//set the polygon mode to GL_FILL
			GetWorld()->GetRenderingSystem()->SetPolygonMode(BoxRenderingID, GL_FILL);
		}
	}

	////check if the 2 key was just released
	//if (PlayerInput.WasReleased(GLFW_KEY_2))
	//{
	//	//check if the polygon mode is GL_FILL
	//	if (GetWorld()->Renderer.BSplineTensorProductSurfacePolygonMode == GL_FILL)
	//	{
	//		//set the polygon mode to GL_LINE
	//		GetWorld()->Renderer.BSplineTensorProductSurfacePolygonMode = GL_LINE;
	//	}
	//	else
	//	{
	//		//set the polygon mode to GL_FILL
	//		GetWorld()->Renderer.BSplineTensorProductSurfacePolygonMode = GL_FILL;
	//	}
	//}
}

void PlayerControllerComponent::ProcessMouseInput(const float XPos, const float YPos) const
{
	//process the mouse movement
	GetWorld()->GetPlayerCamera()->ProcessMouseInput(XPos, YPos, true);
}

void PlayerControllerComponent::ProcessMouseScroll(const float yoffset) const
{
	GetWorld()->GetPlayerCamera()->ProcessMouseScroll(yoffset);
}

void PlayerControllerComponent::Tick(const float DeltaTime)
{
	//check if the w key is pressed
	if (PlayerInput.IsPressed(GLFW_KEY_W))
	{
		GetWorld()->GetPlayerCamera()->Position += GetWorld()->GetPlayerCamera()->Front * DeltaTime * GetWorld()->GetPlayerCamera()->Speed;
	}

	//check if the a key is pressed
	if (PlayerInput.IsPressed(GLFW_KEY_A))
	{
		GetWorld()->GetPlayerCamera()->Position -= GetWorld()->GetPlayerCamera()->Right * DeltaTime * GetWorld()->GetPlayerCamera()->Speed;
	}

	//check if the s key is pressed
	if (PlayerInput.IsPressed(GLFW_KEY_S))
	{
		GetWorld()->GetPlayerCamera()->Position -= GetWorld()->GetPlayerCamera()->Front * DeltaTime * GetWorld()->GetPlayerCamera()->Speed;
	}

	//check if the d key is pressed
	if (PlayerInput.IsPressed(GLFW_KEY_D))
	{
		GetWorld()->GetPlayerCamera()->Position += GetWorld()->GetPlayerCamera()->Right * DeltaTime * GetWorld()->GetPlayerCamera()->Speed;
	}

	//update the camera
	GetWorld()->GetPlayerCamera()->updateCameraVectors();

	//reset the keys data
	PlayerInput.ResetKeysData();
}
