#include "MeshComponent.h"

MeshComponent::MeshComponent(const int InRenderingID): RenderingID(InRenderingID)
{
}

MeshComponent::MeshComponent(Entity* InOwner, const int InRenderingID): Component(InOwner), RenderingID(InRenderingID)
{

}
