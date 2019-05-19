#pragma once

#ifndef __GUI_HIERARCHY_H__
#define __GUI_HIERARCHY_H__

#include <glm/glm.hpp>
#include "GUIBase.h"
#include "EntityManager.h"
#include "Entity.h"

class GUIHierarchy : public GUIBase
{
public:
	GUIHierarchy();
	GUIHierarchy(EntityManager* aEntityManager);
	GUIHierarchy(EntityManager* aEntityManager, const glm::vec2& aPosition, const glm::vec2& aSize);
	GUIHierarchy(const glm::vec2& aPosition, const glm::vec2& aSize);
	~GUIHierarchy();

	void draw() override;

	void setEntityManager(EntityManager* aEntityManager);

	Entity* getActiveEntitiy();

private:
	EntityManager* mEntityManager;
	Entity* mActiveEntity;
};

#endif // !__GUI_HIERARCHY_H__