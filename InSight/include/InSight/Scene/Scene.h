#pragma once

#include <vector>
#include "InSight/Entity/Entity.h"

namespace InSight
{
	struct SceneData
	{
		std::string Name;
		std::vector<int> Entities;
	};

	class Scene
	{
	public: 
		Scene(const std::string& aName);
		~Scene();

		void AddEntity();
		void AddEntity(Entity* aEntity);
		void DeleteEntity(Entity* aEntity);

		void Update();

		void Save();
		SceneData Load(const std::string& aFileName);

	private:
		std::string mName;
		std::vector<Entity*> mEntities;
	};
}