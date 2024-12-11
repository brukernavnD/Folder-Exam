#pragma once
#include "../Components/CameraComponent.h"
#include "../Entities/Entity.h"
#include "../Components/PlayerControllerComponent.h"
#include "../Components/TransformComponent.h"

//the class id for the player
constexpr int PlayerClassID = 0;

class Player : public Entity
{
public:

	//the player controller component for the player
	PlayerControllerComponent PlayerController = PlayerControllerComponent();

	//the camera component for the player
	CameraComponent PlayerCamera = CameraComponent();

	//the transform of the player
	TransformComponent Transform = TransformComponent();

	//constructor(s)
	Player();
	Player(glm::vec3 InPos, glm::vec3 InSize);
};

