#pragma once

#include "InSight/Log.h"

#include <vector>
#include <memory>

#include "InSight/Component/Component.h"


namespace InSight
{
	class EntityManager;

	struct EntityData
	{
		std::string Name;
		std::vector<int> Components;
	};

	class Entity
	{
	public:
		//Constructor
		Entity(int aUniqueID, EntityManager* aEntityManager, const char* aID = "Defulat");
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
			for (auto& component : mComponents)
			{
				T* tempT = dynamic_cast<T*>(component);
				if (tempT != nullptr)
				{
					return true;
				}
			}
			return false;
			//return componentBitset[getComponentTypeID<T>()];
		}

		//get all components this entity has 
		std::vector<Component*>*	getAllComponents()
		{
			return &mComponents;
		}

		//add a new component to this entity
		template <typename T, typename... TArgs>
		T*							addComponent(TArgs&&... mArgs)
		{
			if (mComponents.size() < 2 && !hasComponent<T>())
			{
				T* c = new T(std::forward<TArgs>(mArgs)...);
				c->entity = this;
				mComponents.emplace_back(c);

				//componentArray[getComponentTypeID<T>()] = c;
				//componentBitset[getComponentTypeID<T>()] = true;

				c->init();
				return c;
			}
			return nullptr;
		}

		//get a component from this entity
		template<typename T>
		T*							getComponent() const
		{
			for (auto& component : mComponents)
			{
				T* tempT = dynamic_cast<T*>(component);
				if (tempT != nullptr)
				{
					return dynamic_cast<T*>(component);
				}
			}

			return nullptr;
			/*auto ptr(componentArray[getComponentTypeID<T>()]);
			return dynamic_cast<T*>(ptr);*/
		}

		int					Save();
		void						Load();

	private:
		//pointer to entity manager
		EntityManager* mEntityManager;
		//is this entity active
		bool active = true;
		//id
		std::string id = "-1";
		//vector of all components
		std::vector<Component*> mComponents;
		int mUnqiueID;

	};
}