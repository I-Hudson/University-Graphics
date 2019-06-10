#include "GUI/GUIToolBar.h"
#include "GUI/GUI.h"
#include "GUI/GUIHierarchy.h"

#include "InSight/Component/TransformComponent.h"
#include "InSight/Component/PointLight.h"

namespace InSight
{
	GUIToolBar::GUIToolBar()
	{
	}

	GUIToolBar::GUIToolBar(EntityManager* aEntityManager)
		:mEntityManager(aEntityManager)
	{
	}

	GUIToolBar::GUIToolBar(EntityManager* aEntityManager, const glm::vec2 & aPosition, const glm::vec2 & aSize)
		: mEntityManager(aEntityManager), GUIBase(aPosition, aSize)
	{

	}

	GUIToolBar::GUIToolBar(const glm::vec2& aPosition, const glm::vec2& aSize)
		: GUIBase(aPosition, aSize)
	{
	}

	GUIToolBar::~GUIToolBar()
	{
	}

	void GUIToolBar::init(GUI* aGUI)
	{
		//set gui and hierarchy
		setGUI(aGUI);
		mGUIHierarchy = getGUI()->getGUIElement<GUIHierarchy>();
	}

	void GUIToolBar::draw()
	{
		//set window pos and size
		ImGui::SetNextWindowPos(mPosition);
		ImGui::SetNextWindowSize(mSize);

		//begin next gui element
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("Entity"))
		{
			if (ImGui::MenuItem("Add Entity"))
			{
				//add enttiy
				mEntityManager->addEntity();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Components"))
		{
			if (ImGui::MenuItem("Add Transform Component"))
			{
				mGUIHierarchy->getActiveEntitiy()->addComponent<TransformComponent>();
			}

			if (ImGui::MenuItem("Add Point Light Component"))
			{
				mGUIHierarchy->getActiveEntitiy()->addComponent<PointLight>();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}