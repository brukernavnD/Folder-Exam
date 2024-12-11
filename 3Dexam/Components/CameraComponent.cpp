#include "CameraComponent.h"
#include "../Core/Helpers.h"

CameraComponent::CameraComponent(glm::vec3 position, glm::vec3 up, float yaw, float pitch): Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up), Yaw(yaw), Pitch(pitch), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	// mouse movement (from https://learnopengl.com/Getting-started/Camera)
	LastX = WindowWidth / 2.0f;
	LastY = WindowHeight / 2.0f;
	IsFirstMouse = true;

	updateCameraVectors();
};

glm::mat4 CameraComponent::GetViewMatrix() const
{
	return lookAt(Position, Position + Front, Up);
}

void CameraComponent::ProcessMouseInput(const float XPos, const float YPos, const bool constrainPitch)
{
	if (IsFirstMouse)
	{
		LastX = XPos;
		LastY = YPos;
		IsFirstMouse = false;
	}

	const double Xoffset = XPos - LastX;
	const double Yoffset = LastY - YPos; // reversed since y-coordinates go from bottom to top

	LastX = XPos;
	LastY = YPos;

	//process the mouse movement for the first person camera
	ProcessMouseInputFirstPerson(Xoffset, Yoffset, constrainPitch);
}

void CameraComponent::ProcessMouseInputFirstPerson(float xoffset, float yoffset, const bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw   += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void CameraComponent::ProcessMouseScroll(const float InValue)
{
	//Zoom -= InValue;
	//if (Zoom < 1.0f)
	//	Zoom = 1.0f;
	//if (Zoom > 45.0f)
	//	Zoom = 45.0f;

	//add the yoffset to the speed
	Speed += InValue * SpeedAdjustmentScalar;

	//check if the speed is less than the minimum speed
	if (Speed < MinSpeed)
	{
		//set the speed to the minimum speed
		Speed = MinSpeed;
	}

	//check if the speed is greater than the maximum speed
	if (Speed > MaxSpeed)
	{
		//set the speed to the maximum speed
		Speed = MaxSpeed;
	}
}

void CameraComponent::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = normalize(front);
	// also re-calculate the Right and Up vector
	Right = normalize(cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up    = normalize(cross(Right, Front));
}
