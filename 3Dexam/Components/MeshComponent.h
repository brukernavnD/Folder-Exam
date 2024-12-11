#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	//the rendering id for this component
	int RenderingID;

	//constructor(s)
	MeshComponent() = default;
	explicit MeshComponent(int InRenderingID);
	explicit MeshComponent(Entity* InOwner, int InRenderingID);
};

