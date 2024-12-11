#include "StaticEntity.h"
#include "../Components/TransformComponent.h"

StaticEntity::StaticEntity() = default;

StaticEntity::StaticEntity(const int InRenderingID, const glm::vec3 InPosition, const glm::vec3 InScale) : Entity("StaticEntity"), TransformComp(this, InPosition, InScale), MeshComp(this, InRenderingID)
{
	//set the class id of the static entity
	ClassID = StaticEntityClassID;

	//set the list of components
	Components.push_back(&MeshComp);
	Components.push_back(&TransformComp);

}