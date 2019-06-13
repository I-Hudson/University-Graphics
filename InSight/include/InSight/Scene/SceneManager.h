#pragma once

#include <vector>

namespace InSight
{
	class Scene;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		inline static SceneManager& Get() { return *sInstance; }

		std::vector<Scene*> const& GetAllScenes() const;

		Scene* AddScene(const std::string& aSceneName);
		Scene* GetScene(const std::string& aSceneName);
		Scene* GetActiveScene() const;

		void LoadSceneFromFile(const std::string& aScenePath);

		void Destroy();

	private:
		std::vector<Scene*> mScenes;
		Scene* mActiveScene;
		static SceneManager* sInstance;
	};
}