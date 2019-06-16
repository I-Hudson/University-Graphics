#pragma once

#include <vector>
#include "InSight/Entity/Entity.h"

namespace InSight
{
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
		void Load();

	private:
		std::string mName;
		std::vector<Entity*> mEntities;
	};
}