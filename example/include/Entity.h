#pragma once

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Log.h"

#include <vector>
#include <memory>

#include "Component.h"

class EntityManager;

class Entity
{
public:
	//Constructor
	Entity(EntityManager* aEntityManager, const char* aID = "Defulat");
	//Destructor
	~Entity();

	//update
	void						update();
	//draw
	void						draw();

	//set ID for this entity
	void						setID(std::string aID);
	//get the ID for this entity
	std::string					getID();

	//is this entity active
	bool						isActive() const { return active; }
	//destroy this entity
	void						destroy();

	//get the entity manager
	EntityManager*				getEntityManger();

	//check if this entity has a component 
	template <typename T> 
	bool						hasComponent() const
	{
		return componentBitset[getComponentTypeID<T>()];
	}

	//get all components this entity has 
	std::vector<Component*>*	getAllComponents()
	{
		return &mComponents;
	}

	//add a new component to this entity
	template <typename T, typename... TArgs>
	T&							addComponent(TArgs&&... mArgs)
	{
		T* c = DEBUG_NEW T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		mComponents.emplace_back(c);

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	//get a component from this entity
	template<typename T>
	T&							getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *dynamic_cast<T*>(ptr);
	}

private:
	//pointer to entity manager
	EntityManager* mEntityManager;
	//is this entity active
	bool active = true;
	//id
	std::string id = "-1";
	//vector of all components
	std::vector<Component*> mComponents;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
};
#endif // __ENTITY_H__