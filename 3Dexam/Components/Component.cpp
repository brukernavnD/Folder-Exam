#include "Component.h"

Component::Component() = default;

Component::Component(Entity* InOwner)
{
	Owner = InOwner;
}

Component::~Component() = default;
