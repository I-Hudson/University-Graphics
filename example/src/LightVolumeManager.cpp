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
	for (auto kv : mLightVolumes)
	{
		kv->update();
	}
}

void LightVolumeManager::addLightVolume(LightComponent* aComponent)
{
	mLightVolumes.emplace_back(aComponent);
}

std::vector<LightComponent*>* LightVolumeManager::getAllVolumes()
{
	return &mLightVolumes;
}
