#pragma once

#include <vector>
#include "InSight/Entity/Entity.h"
namespace InSight
{
	class Scene
	{
	public: 
		Scene();
		~Scene();

		void AddEntity();
		void DeleteEntity();

	private:
		std::vector<Entity*> mEntities;
	};
}