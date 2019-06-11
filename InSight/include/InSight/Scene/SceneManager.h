#pragma once

#include <vector>

class Scene;

namespace InSight
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		inline static SceneManager& Get() { return *sInstance; }

		std::vector<Scene*> const& GetAllScenes() const;

		Scene* AddScene(const std::string& aSceneName);
		Scene* GetScene(const std::string& aSceneName);

		void LoadSceneFromFile(const std::string& aScenePath);

		void Destroy();

	private:
		std::vector<Scene*> mScenes;
		Scene* mActiveScene;
		static SceneManager* sInstance;
	};
}