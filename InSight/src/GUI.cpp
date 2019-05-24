#include "GUI.h"
#include "GUIBase.h"

GUI::GUI()
{
	//reserve space in vector
	mGUIElements.reserve(2);
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
	//draw all elements
	for (auto& element : mGUIElements)
	{
		element->draw();
	}
}
