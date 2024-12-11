#pragma once
#include <string>
#include "../Core/Helpers.h"

//forward declaration(s)
class Component;

class Entity
{
public:

	//the components of the entity
	std::vector<Component*> Components;

	//the class id of the object class
	int ClassID = -1;

	//the id of this object (defined with a preprocessor macro)
	int ID = -1;

	//our name
	std::string Name = "UnNamed";

	//constructor(s)
	Entity();
	explicit Entity(std::string InName);

	//destructor(s)
	virtual ~Entity();

	//operator overload(s)
	bool operator==(const Entity* Other) const;

	//template function to get a component of a specific type
	template <typename T>
	T* GetComponent()
	{
		//iterate over the components
		for (Component* Comp : Components)
		{
			//try to cast the component to the desired type
			T* CastedComp = dynamic_cast<T*>(Comp);

			//if the cast was successful, return the component
			if (CastedComp)
			{
				return CastedComp;
			}
		}
		//return null if the component was not found
		return nullptr;
	}
};