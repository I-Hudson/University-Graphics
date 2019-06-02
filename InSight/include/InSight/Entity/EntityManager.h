#pragma once

#ifndef __ENTIT_MANAGER_H__
#define __ENTIT_MANAGER_H__

#include <vector>
#include <memory>
#include "InSight/Entity/Entity.h"

//class Entity;

class EntityManager
{
public:
	//constructor
	EntityManager();
	//destructor
	~EntityManager();

	//update and draw
	void						update();
	void						draw();

	//get all entities
	std::vector<Entity*> const& getEntities() const;

	//Add a new entity to entites
	Entity*						addEntity();

	/*
		Return an entity with a ID
	*/
	Entity*						getEntity(std::string aID);

	//get compoent on all entities
	template<typename T>
	std::vector<T*> getAllEntityComponents()
	{
		std::vector<T*> tempV;

		for (auto& e : mEntities)
		{
			if (e->hasComponent<T>())
			{
				T* tempT = e->getComponent<T>();
				tempV.push_back(tempT);
			}
		}
		return tempV;
	}

	//destroy 
	void						 destroy();

private:
	//entities
	std::vector<Entity*> mEntities;
};

#endif // !__ENTIT_MANAGER_H__
