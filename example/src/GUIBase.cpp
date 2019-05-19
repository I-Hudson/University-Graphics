#include "GUIBase.h"

GUIBase::GUIBase()
{
}

GUIBase::GUIBase(const glm::vec2 & aPosition, const glm::vec2 & aSize)
{
	mPosition = ImVec2(aPosition.x, aPosition.y);
	mSize = ImVec2(aSize.x, aSize.y);
}

GUIBase::~GUIBase()
{
}

void GUIBase::init(GUI * aGUI)
{
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
	mGUI = aGUI;
}

GUI* GUIBase::getGUI()
{
	return mGUI;
}
