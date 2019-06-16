#include "GUI/GUIToolBar.h"
#include "GUI/GUI.h"
#include "GUI/GUIHierarchy.h"

#include "InSight/Component/TransformComponent.h"
#include "InSight/Component/PointLight.h"
#include "InSight/Component/AmbientLight.h"
//#include "InSight/Component/CameraComponent.h"
#include "InSight/Component/DirectionalLight.h"
#include "InSight/Component/MeshComponent.h"
#include "InSight/Component/SpotLight.h"

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

			if (mGUIHierarchy->getActiveEntitiy() != nullptr && ImGui::MenuItem("Delete Entity"))
			{
				mEntityManager->DeleteEntity(mGUIHierarchy->getActiveEntitiy());
				mGUIHierarchy->SetActiveEntityToNull();
			}
			ImGui::EndMenu();
		}

		if (mGUIHierarchy->getActiveEntitiy() != nullptr)
		{
			if (ImGui::BeginMenu("Components"))
			{
				if (ImGui::MenuItem("Add Transform Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<TransformComponent>();
				}
				if (ImGui::MenuItem("Add Ambient Light Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<AmbientLight>();
				}
				if (ImGui::MenuItem("Add Directional Light Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<DirectionalLight>();
				}
				if (ImGui::MenuItem("Add Mesh Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<MeshComponent>();
				}
				if (ImGui::MenuItem("Add Point Light Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<PointLight>();
				}
				if (ImGui::MenuItem("Add Spot Light Component"))
				{
					mGUIHierarchy->getActiveEntitiy()->addComponent<SpotLight>();
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}