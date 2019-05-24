#include "GUIHierarchy.h"

GUIHierarchy::GUIHierarchy()
{
}

GUIHierarchy::GUIHierarchy(EntityManager* aEntityManager)
{
	//set entiy manager
	mEntityManager = aEntityManager;
}

GUIHierarchy::GUIHierarchy(EntityManager* aEntityManager, const glm::vec2& aPosition, const glm::vec2& aSize) :
	GUIBase(aPosition, aSize)
{
	//set entiy manager
	mEntityManager = aEntityManager;
}

GUIHierarchy::GUIHierarchy(const glm::vec2& aPosition, const glm::vec2& aSize) :
	GUIBase(aPosition, aSize)
{
}

GUIHierarchy::~GUIHierarchy()
{
}

void GUIHierarchy::draw()
{
	//set window pos and size
	ImGui::SetNextWindowPos(mPosition);
	ImGui::SetNextWindowSize(mSize);

	//begin next gui element
	ImGui::Begin("GUI Hierarchy");

	//for each entitiy in scene add here
	if (mEntityManager != nullptr)
	{
		for (auto& entity : mEntityManager->getEntities())
		{
			if (ImGui::Button(entity->getID().c_str(), ImVec2(mSize.x * 1.0f, mSize.y * 0.15f)))
			{
				if (mActiveEntity != entity)
				{
					mActiveEntity = entity;
					std::string log("Active Entitiy has changed: ");
					log += mActiveEntity->getID();
					EN_TRACE(log);
				}
			}
		}
	}
	else
	{
		ImGui::Text("No Entity Manager set.");
	}

	ImGui::End();
}

void GUIHierarchy::setEntityManager(EntityManager* aEntityManager)
{
	//set entiy manager
	mEntityManager = aEntityManager;
}

Entity* GUIHierarchy::getActiveEntitiy()
{
	//get pointer to active entity 
	return mActiveEntity;
}