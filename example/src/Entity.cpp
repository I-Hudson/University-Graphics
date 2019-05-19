#include "Entity.h"

Entity::Entity(EntityManager* aEntityManager, const char* aID /*= "Defualt" */)
	: mEntityManager(aEntityManager)
{
	setID(aID);
}

Entity::~Entity()
{
	for (auto& c : mComponents)
	{
		delete c;
	}
}

void Entity::update()
{
	for (auto& c : mComponents) c->update();
}

void Entity::draw()
{
	for (auto& c : mComponents) c->draw(nullptr, true);
}

void Entity::setID(std::string aID)
{
	id = aID;
}

std::string Entity::getID()
{
	return id;
}

void Entity::destroy()
{
	for (unsigned int i = 0; i < mComponents.size(); ++i)
	{
		mComponents[i]->destroy();
		delete mComponents[i];
	}
	mComponents.clear();
	componentArray.empty();
	componentBitset.reset();
}

EntityManager* Entity::getEntityManger()
{
	return mEntityManager;
}
