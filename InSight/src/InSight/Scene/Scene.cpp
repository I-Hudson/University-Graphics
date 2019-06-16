#include "InSight/Scene/Scene.h"
#include <iostream>
#include <fstream>

namespace InSight
{
	Scene::Scene(const std::string& aName)
		:mName(aName)
	{
	}

	Scene::~Scene()
	{
		//std::string fileName(mName);
		//fileName.append(".txt");

		//std::ofstream file;
		//file.open(fileName.c_str());
		//if (!file.is_open())
		//{
		//	file.open(fileName, std::ios::out);
		//}
		//file.open(fileName, std::ios::in | std::ios::out);

		//file << "---\n";
		//file << "Scene Settings:\n";
		//file << "	SceneName:" << mName.c_str() << "\n";
		//file << "---\n";

		//for (auto& entity : mEntities)
		//{
		//	entity->Save();
		//	delete entity;
		//}
		//file.close();
		//mEntities.clear();
		mEntities.clear();
	}

	void Scene::AddEntity()
	{
	}

	void Scene::AddEntity(Entity* aEntity)
	{
		mEntities.push_back(aEntity);
	}

	void Scene::DeleteEntity(Entity* aEntity)
	{
		for (int i = 0; i < mEntities.size(); i++)
		{
			if (mEntities[i] == aEntity)
			{
				mEntities.erase(mEntities.begin() + i);
				return;
			}
		}
	}

	void Scene::Update()
	{
		for (auto& entity : mEntities)
		{
			entity->update();
		}
	}

	void Scene::Save()
	{
		std::string filePath("Scenes/");
		filePath.append(mName);
		filePath.append(".txt");

		std::ofstream file;
		file.open(filePath.c_str(), std::ios::out);
		if (!file.fail())
		{
			//file.open(fileName, std::ios::out);
			EN_CORE_INFO("File created: {0}!", filePath.c_str());
		}

		file << "---\n";
		file << "Scene Settings:\n";
		file << "	SceneName:" << mName.c_str() << "\n";

		for (auto& entity : mEntities)
		{
			entity->Save(file);
		}

		file << "---\n";

		file.close();
	}

	void Scene::Load()
	{
	}
}