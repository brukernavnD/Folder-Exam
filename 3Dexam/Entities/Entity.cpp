#include "../Entities/Entity.h"
#include "../Components/Component.h"
#include "../Systems/WorldSystem.h"

Entity::~Entity() = default;

Entity::Entity() = default;

Entity::Entity(std::string InName) : Name(std::move(InName))
{
	//get our id
	ID = IDCounter();
}

bool Entity::operator==(const Entity* Other) const
{
	return ID == Other->ID;
}