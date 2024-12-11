#include "Player.h"

Player::Player() : Entity("Player")
{
	ClassID = PlayerClassID;
}

Player::Player(const glm::vec3 InPos, const glm::vec3 InSize) : Entity("Player")
{
	//set the class id of the player
	ClassID = PlayerClassID;

	//set the position of the player
	Transform.Position = InPos;

	//set the size of the player
	Transform.Scale = InSize;

	//add the components to the player
	Components.push_back(&PlayerController);
	Components.push_back(&PlayerCamera);
	Components.push_back(&Transform);
}
