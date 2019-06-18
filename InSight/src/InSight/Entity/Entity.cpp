#include "Entity/Entity.h"

namespace InSight
{
	Entity::Entity(int aUniqueID, EntityManager* aEntityManager, const char* aID /*= "Defualt" */)
		: mUnqiueID(aUniqueID), mEntityManager(aEntityManager)
	{
		//set id
		setID(aID);
	}

	Entity::~Entity()
	{
		//destroy all components
		for (auto& c : mComponents)
		{
			delete c;
		}
	}

	void Entity::update()
	{
		//update all components
		for (auto& c : mComponents) c->update();
	}

	void Entity::draw()
	{
		//draw all components
		for (auto& c : mComponents) c->draw(nullptr, true);
	}

	void Entity::setID(std::string aID)
	{
		//set id
		id = aID;
	}

	std::string Entity::getID()
	{
		//get id
		return id;
	}

	void Entity::destroy()
	{
		//destroy entity and components
		for (unsigned int i = 0; i < mComponents.size(); ++i)
		{
			mComponents[i]->destroy();
			delete mComponents[i];
		}
		mComponents.clear();
		//componentArray.empty();
		//componentBitset.reset();
	}

	EntityManager* Entity::getEntityManger()
	{
		//get entity manager
		return mEntityManager;
	}

	int Entity::Save()
	{
		EntityData data = {};
		data.Name = getID();

		for (auto& com : mComponents)
		{
			data.Components.push_back(com->Save());
		}
		return mUnqiueID;
	}

	void Entity::Load()
	{
	}
}