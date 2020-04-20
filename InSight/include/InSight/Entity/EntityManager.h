#pragma once

#include <vector>
#include <memory>
#include "InSight/Entity/Entity.h"

//class Entity;

namespace InSight
{
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

		inline static EntityManager& Get() { return *sInstance; }
		inline static void Reset() { delete sInstance; }

		//get all entities
		std::vector<Entity*> const& getEntities() const;

		//Add a new entity to entites
		Entity*		addEntity();

		/*
			Return an entity with a ID
		*/
		Entity*		getEntity(std::string aID);

		//Delete Entity
		void						DeleteEntity(Entity* aEntity);

		//get compoent on all entities
		template<typename T>
		std::vector<T*> getAllEntityComponents()
		{
			std::vector<T*> tempV;

			if (mEntities.size() > 0)
			{
				for (auto& e : mEntities)
				{
					if (e->hasComponent<T>())
					{
						T* tempT = e->getComponent<T>();
						tempV.push_back(tempT);
					}
				}
			}
			return tempV;
		}

		//destroy 
		void						 destroy();

	private:
		//entities
		std::vector<Entity*> mEntities;

		static EntityManager* sInstance;
	};
}