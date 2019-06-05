#pragma once

#ifndef __GUI_HIERARCHY_H__
#define __GUI_HIERARCHY_H__

#include <glm/glm.hpp>
#include "GUIBase.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"

class GUIHierarchy : public GUIBase
{
public:
	//constructors
	GUIHierarchy();
	GUIHierarchy(EntityManager* aEntityManager);
	GUIHierarchy(EntityManager* aEntityManager, const glm::vec2& aPosition, const glm::vec2& aSize);
	GUIHierarchy(const glm::vec2& aPosition, const glm::vec2& aSize);
	//destructor
	~GUIHierarchy();

	//override
	void draw() override;

	//set entity manager reference
	void setEntityManager(EntityManager* aEntityManager);

	//get current active entitiy in hierarchy 
	Entity* getActiveEntitiy();

private:
	//entity manager refernce
	EntityManager* mEntityManager;
	//currnet active entity
	Entity* mActiveEntity;
};

#endif // __GUI_HIERARCHY_H__