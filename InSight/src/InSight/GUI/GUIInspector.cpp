#include "GUI/GUIInspector.h"
#include "GUI/GUI.h"
#include "GUI/GUIHierarchy.h"

namespace InSight::GUI
{
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
		ImGui::Begin("GUI Inspector", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		ImGui::TextColored(ImVec4(170, 0, 0, 1), "Selected Entity:");

		if (mHierarchy != nullptr && mHierarchy->getActiveEntitiy() != nullptr)
		{
			char text[32] = "Test Text";
			std::strcpy(text, mHierarchy->getActiveEntitiy()->getID().c_str());
			ImGui::InputText("Name", text, IM_ARRAYSIZE(text));
			mHierarchy->getActiveEntitiy()->setID(text);

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
}