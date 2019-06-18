#pragma once

#include "imgui.h"
#include "Log.h"
#include <glm/glm.hpp>

namespace InSight::GUI
{
	class GUI;

	class GUIBase
	{
	public:
		//constructor
		GUIBase();
		GUIBase(const glm::vec2& aPosition, const glm::vec2& aSize);
		//destructor
		~GUIBase();

		//overrides
		virtual void init(GUI* aGUI);
		virtual void draw();
		virtual void destroy();

		//set parent gui
		void setGUI(GUI* aGUI);

		//get parent gui
		GUI* getGUI();

	protected:
		//position and size
		ImVec2 mPosition;
		ImVec2 mSize;

	private:
		//parnet gui
		GUI* mGUI;
	};
}