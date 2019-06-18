#pragma once

#include <glm/glm.hpp>
#include "GUI/GUIBase.h"

namespace InSight::GUI
{
	class GUIHierarchy;

	class GUIInspector : public GUIBase
	{
	public:

		//constructor
		GUIInspector();
		GUIInspector(const glm::vec2& aPosition, const glm::vec2& aSize);
		//destructor
		~GUIInspector();

		//overrides
		void init(GUI* aGUI) override;
		//draw elements for this gui element
		void draw() override;

		//Set the hierarchy pointer
		void setHierarchy(GUIHierarchy* aHierarchy);

	private:
		//hierarchy pointer
		GUIHierarchy* mHierarchy;
	};
}