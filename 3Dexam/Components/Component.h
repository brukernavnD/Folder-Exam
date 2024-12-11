#pragma once

class Entity;

class Component
{
public:

	//constructor(s)
	Component();
	explicit Component(Entity* InOwner);

	//destructor(s)
	virtual ~Component();

	//pointer to the entity that the component is attached to
	Entity* Owner;

	//template function to check if the component is of a certain type
	template <typename T>
	bool IsA() const
	{
		//return whether or not the component is of the type
		return dynamic_cast<const T*>(this) != nullptr;
	}

};

