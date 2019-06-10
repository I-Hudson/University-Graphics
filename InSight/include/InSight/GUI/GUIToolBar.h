#pragma once

#include <glm/glm.hpp>
#include "GUIBase.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"

class GUIHierarchy;

namespace InSight
{
	class GUIToolBar : public GUIBase
	{
	public:
		//constructors
		GUIToolBar();
		GUIToolBar(EntityManager* aEntityManager);
		GUIToolBar(EntityManager* aEntityManager, const glm::vec2& aPosition, const glm::vec2& aSize);
		GUIToolBar(const glm::vec2& aPosition, const glm::vec2& aSize);
		//destructor
		~GUIToolBar();

		//override
		void init(GUI* aGUI) override;
		//override
		void draw() override;

	private:
		//entity manager refernce
		EntityManager* mEntityManager;

		GUIHierarchy* mGUIHierarchy;
	};
}