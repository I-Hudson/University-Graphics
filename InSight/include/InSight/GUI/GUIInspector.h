#pragma once

#ifndef __GUI_INSPECTOR_H__
#define __GUI_INSPECTOR_H__

#include <glm/glm.hpp>
#include "GUI/GUIBase.h"

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
#endif // __GUI_INSPECTOR_H__
