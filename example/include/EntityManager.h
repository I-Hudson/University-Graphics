#pragma once

#ifndef __ENTIT_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include <vector>
#include <memory>
#include "Entity.h"

//class Entity;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void						update();
	void						draw();

	void						refresh();
	std::vector<Entity*> const& getEntities() const;

	//Add a new entity to entites
	Entity*						addEntity();

	/*
		Return an entity with a ID
	*/
	Entity*						getEntity(std::string aID);

	template<typename T>
	std::vector<T*> getAllEntityComponents()
	{
		std::vector<T*> tempV;

		for (auto& e : mEntities)
		{
			if (e->hasComponent<T>())
			{
				T* tempT = &e->getComponent<T>();
				tempV.push_back(tempT);
			}
		}
		return tempV;
	}

	void						 destroy();

private:
	std::vector<Entity*> mEntities;
};

#endif // !__ENTIT_MANAGER_H__
