#include "BoxEntity.h"

#include "../Data/BoxRenderingData.h"

BoxEntity::BoxEntity() : Entity("Box")
{
	//set the class id of the box
	ClassID = BoxEntityClassID;

	//set the list of components
	Components.push_back(&TransformComp);
	Components.push_back(&MeshComp);
}

BoxEntity::BoxEntity(const glm::vec3 InPosition, const glm::vec3 InScale) : Entity("Box"), TransformComp(this, InPosition, InScale), MeshComp(this, BoxRenderingID)
{
	//set the class id of the box
	ClassID = BoxEntityClassID;

	//set the list of components
	Components.push_back(&TransformComp);
	Components.push_back(&MeshComp);
}
