#include "KillComponent.h"

KillComponent::KillComponent() = default;

KillComponent::KillComponent(Entity* InOwner, const float InKillY): Component(InOwner), KillY(InKillY)
{
	
}
