#include "GUI/GUIBase.h"

namespace InSight::GUI
{
	GUIBase::GUIBase()
	{
	}

	GUIBase::GUIBase(const glm::vec2 & aPosition, const glm::vec2 & aSize)
	{
		//set position and size
		mPosition = ImVec2(aPosition.x, aPosition.y);
		mSize = ImVec2(aSize.x, aSize.y);
	}

	GUIBase::~GUIBase()
	{
	}

	void GUIBase::init(GUI* aGUI)
	{
		//set gui
		setGUI(aGUI);
	}

	void GUIBase::draw()
	{
	}

	void GUIBase::destroy()
	{
	}

	void GUIBase::setGUI(GUI* aGUI)
	{
		//set gui
		mGUI = aGUI;
	}

	GUI* GUIBase::getGUI()
	{
		//get gui pointer
		return mGUI;
	}
}