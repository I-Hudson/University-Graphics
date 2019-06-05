#pragma once

#ifndef __LIGHT_VOLUME_MANAGER_H__
#define __LIGHT_VOLUME_MANAGER_H__

#include <vector>
#include "Component/LightComponent.h"
#include "Entity/Entity.h"

class LightVolumeManager 
{
public:
	//constructor
	LightVolumeManager();
	//destructor
	~LightVolumeManager();

	//getLightVolume(Name);
	//template<typename T>
	//T* getLightVolume(const std::string aID)
	//{
	//	if (mLightVolumes.size() > 0)
	//	{
	//		return dynamic_cast<T*>(mLightVolumes[aID]);
	//	}
	//	return nullptr;
	//}
	//getLightVolumes<Type>
	//template<typename T>
	//std::map<std::string, T*> getLightVolumes()
	//{
	//	std::map<std::string, T*> tempMap;
	//	for (auto& kv : mLightVolumes)
	//	{
	//		T* tempT = dynamic_cast<T*>(kv.second);
	//		if (tempT != nullptr)
	//		{
	//			tempMap
	//		}
	//	}
	//}

	//update
	void update();

	//get all light volumes
	std::vector<LightComponent*>* getAllVolumes();

	//add new light volumes
	void addLightVolume(LightComponent* aComponent);

private:
	//light volumes
	std::vector<LightComponent*> mLightVolumes;
};

#endif // __LIGHT_VOLUME_MANAGER_H__
