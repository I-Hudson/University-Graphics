#include "InSight/Scene/Scene.h"
#include <iostream>
#include <fstream>

#include "InSight/Component/TransformComponent.h"

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

		std::ofstream oFile(filePath, std::ios::binary);

		SceneData data = {};
		data.Name = mName;

		for (auto& e : mEntities)
		{
			data.Entities.push_back(e->Save());
		}
	
		oFile.write((char*)&data, sizeof(data));
		oFile.close();
	}

	SceneData Scene::Load(const std::string& aFileName)
	{
		std::string filePath("Scenes/");
		filePath.append(aFileName);
		filePath.append(".txt");

		std::ifstream iFile(filePath, std::ios::binary);
		SceneData loadData;
		iFile.read((char*)&loadData, sizeof(loadData));
		iFile.close();

		return loadData;
	}
}