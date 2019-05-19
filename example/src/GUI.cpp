#include "GUI.h"
#include "GUIBase.h"

GUI::GUI()
{
	mGUIElements.reserve(3);
}

GUI::~GUI()
{
	//clear all elements
	for (int i = 0; i < mGUIElements.size(); ++i)
	{
		mGUIElements[i]->destroy();
		delete mGUIElements[i];
	}
	mGUIElements.clear();
}

void GUI::update()
{
}

void GUI::draw()
{
	for (auto& element : mGUIElements)
	{
		element->draw();
	}
}
