#include "EntityManager.h"
#include "Entity.h"
#include <iostream>

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	for (auto& entity : mEntities)
	{
		std::cout << "Entity : " << entity->getID().c_str() << " : has been deleted" << "\n";
		delete entity;
	}
}

void EntityManager::update()
{
	for (auto& e : mEntities)
	{
		e->update();
	}
}

void EntityManager::draw()
{
	for (auto& e : mEntities)
	{
		e->draw();
	}
}

void EntityManager::refresh()
{
	mEntities.erase(std::remove_if(std::begin(mEntities), std::end(mEntities),
		[](const Entity* mEntity)
	{
		return !mEntity->isActive();
	}),
		std::end(mEntities));
}

std::vector<Entity*> const & EntityManager::getEntities() const
{
	return mEntities;
}

Entity* EntityManager::addEntity()
{
	Entity *e = DEBUG_NEW Entity(this);
	mEntities.emplace_back(e);
	return e;
}

Entity* EntityManager::getEntity(std::string aID)
{
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
	for (unsigned int i = 0; i < mEntities.size(); ++i)
	{
		mEntities[i]->destroy();
		delete mEntities[i];
	}
	mEntities.clear();
}
