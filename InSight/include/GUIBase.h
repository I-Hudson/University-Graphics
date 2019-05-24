#pragma once

#ifndef __GUI_BASE_H__
#define __GUI_BASE_H__

#include "imgui.h"
#include "Log.h"
#include <glm/glm.hpp>

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

#endif // !__GUI_BASE_H__
