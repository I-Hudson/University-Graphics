#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include <iostream>

#include "InSight/Scene/SceneManager.h"
#include "InSight/Scene/Scene.h"

EntityManager* EntityManager::sInstance = nullptr;

EntityManager::EntityManager()
{
	EN_ASSERT(sInstance, "EntityManager already exists!");
	sInstance = this;
}

EntityManager::~EntityManager()
{
	//delete all entities
	for (auto& entity : mEntities)
	{
		std::cout << "Entity : " << entity->getID().c_str() << " : has been deleted" << "\n";
		delete entity;
	}
}

void EntityManager::update()
{
	//update all entities
	for (auto& e : mEntities)
	{
		e->update();
	}
}

void EntityManager::draw()
{
	//draw all entities
	for (auto& e : mEntities)
	{
		e->draw();
	}
}

std::vector<Entity*> const & EntityManager::getEntities() const
{
	//return pointer to vector with all of the entities
	return mEntities;
}

Entity* EntityManager::addEntity()
{
	//add a new entity
	Entity *e = new Entity(this);
	mEntities.emplace_back(e);
	InSight::SceneManager::Get().GetActiveScene()->AddEntity(e);
	return e;
}

Entity* EntityManager::getEntity(std::string aID)
{
	//get an entity
	for (auto& e : mEntities)
	{
		if (e != nullptr)
		{
			int cc = e->getID().compare(aID);
			if (cc == 0)
			{
				return e;
				break;
			}
		}
	}
	return nullptr;
}

void EntityManager::destroy()
{
	//destroy all entities
	for (unsigned int i = 0; i < mEntities.size(); ++i)
	{
		mEntities[i]->destroy();
		delete mEntities[i];
	}
	mEntities.clear();
}
