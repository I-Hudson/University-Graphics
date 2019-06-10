#include "LightVolumeManager.h"

LightVolumeManager::LightVolumeManager()
{
}

LightVolumeManager::~LightVolumeManager()
{
	//for (auto& kv : mLightVolumes)
	//{
	//	if (kv != nullptr)
	//	{
	//		delete kv;
	//	}
	//}
}

void LightVolumeManager::update()
{
	//update all light volumes
	for (auto kv : mEntityManager->getAllEntityComponents<LightComponent>())
	{
		kv->update();
	}
}

void LightVolumeManager::addLightVolume(LightComponent* aComponent)
{
	//add light volume
	mLightVolumes.emplace_back(aComponent);
}

std::vector<LightComponent*>* LightVolumeManager::getAllVolumes()
{
	//get pointer to vector for all light volume
	return &mLightVolumes;
}
