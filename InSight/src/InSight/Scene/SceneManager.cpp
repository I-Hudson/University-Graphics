#include "InSight/Scene/SceneManager.h"
#include "Log.h"

namespace InSight
{
	SceneManager* SceneManager::sInstance = new SceneManager();
	SceneManager::SceneManager()
	{
		EN_CORE_ASSERT(!SceneManager::sInstance, "Scene Manager already exists!");
	}

	SceneManager::~SceneManager()
	{
	}

	std::vector<Scene*> const& SceneManager::GetAllScenes() const
	{
		std::vector<Scene*> scenes;
		return scenes;
	}

	Scene* SceneManager::AddScene(const std::string& aSceneName)
	{
		return nullptr;
	}

	Scene* SceneManager::GetScene(const std::string& aSceneName)
	{
		return nullptr;
	}

	void SceneManager::LoadSceneFromFile(const std::string& aScenePath)
	{
	}

	void SceneManager::Destroy()
	{
	}
}
