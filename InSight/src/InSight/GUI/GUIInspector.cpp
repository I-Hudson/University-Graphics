#include "GUI/GUIInspector.h"
#include "GUI/GUI.h"
#include "GUI/GUIHierarchy.h"

GUIInspector::GUIInspector()
{
}

GUIInspector::GUIInspector(const glm::vec2 & aPosition, const glm::vec2 & aSize) :
	GUIBase(aPosition, aSize)
{
}

GUIInspector::~GUIInspector()
{
}

void GUIInspector::init(GUI * aGUI)
{
	//set gui and hierarchy
	setGUI(aGUI);
	setHierarchy(getGUI()->getGUIElement<GUIHierarchy>());
}

void GUIInspector::draw()
{
	//set window pos and size
	ImGui::SetNextWindowPos(mPosition);
	ImGui::SetNextWindowSize(mSize);

	//begin next gui element
	ImGui::Begin("GUI Inspector");

	if (mHierarchy != nullptr && mHierarchy->getActiveEntitiy() != nullptr)
	{
		for (auto& component : *mHierarchy->getActiveEntitiy()->getAllComponents())
		{
			component->gui();
		}
	}

	ImGui::End();
}

void GUIInspector::setHierarchy(GUIHierarchy* aHierarchy)
{
	//set hierarchy
	mHierarchy = aHierarchy;
}
