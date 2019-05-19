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
	GUIBase();
	GUIBase(const glm::vec2& aPosition, const glm::vec2& aSize);
	~GUIBase();

	virtual void init(GUI* aGUI);
	virtual void draw();
	virtual void destroy();

	void setGUI(GUI* aGUI);

	GUI* getGUI();

protected:
	ImVec2 mPosition;
	ImVec2 mSize;

private:
	GUI* mGUI;
};

#endif // !__GUI_BASE_H__
