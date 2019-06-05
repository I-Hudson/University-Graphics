//#include "Audio/AudioManager.h"
//
//AudioManager::AudioManager()
//{
//}
//
//AudioManager::~AudioManager()
//{
//}
//
//void AudioManager::init()
//{
//}
//
//void AudioManager::update()
//{
//}
//
//void AudioManager::destroy()
//{
//}
//
//void AudioManager::errorCheck(FMOD_RESULT aResult)
//{
//}
//
//void AudioManager::loadBank(const std::string & strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
//{
//}
//
//void AudioManager::loadEvent(const std::string & strEventName)
//{
//}
//
//void AudioManager::loadsound(const std::string & strSoundName, bool b3d, bool bLooping, bool bStream)
//{
//}
//
//void AudioManager::unLoadSound(const std::string & strSoundName)
//{
//}
//
//void AudioManager::set3dListenerAndOrientation(const glm::vec3 & vPos, float fVolumedB)
//{
//}
//
//int AudioManager::playSounds(const std::string & strSoundName, const glm::vec3 & vPos, float fVolumedB)
//{
//	return 0;
//}
//
//void AudioManager::playEvent(const std::string & strEventName)
//{
//}
//
//void AudioManager::stopChannel(int nChannelId)
//{
//}
//
//void AudioManager::stopEvent(const std::string & strEventName, bool bImmediate)
//{
//}
//
//void AudioManager::geteventParameter(const std::string & strEventName, const std::string & strEventParameter, float * parameter)
//{
//}
//
//void AudioManager::setEventParameter(const std::string & strEventName, const std::string & strParameterName, float fValue)
//{
//}
//
//void AudioManager::stopAllChannels()
//{
//}
//
//void AudioManager::setChannel3dPosition(int nChannelId, const glm::vec3 & vPosition)
//{
//}
//
//void AudioManager::setChannelvolume(int nChannelId, float fVolumedB)
//{
//}
//
//bool AudioManager::isPlaying(int nChannelId) const
//{
//	return false;
//}
//
//bool AudioManager::isEventPlaying(const std::string & strEventName) const
//{
//	return false;
//}
//
//float AudioManager::dbToVolume(float db)
//{
//	return 0.0f;
//}
//
//float AudioManager::volumeTodb(float volume)
//{
//	return 0.0f;
//}
//
//FMOD_VECTOR AudioManager::vectorToFmod(const glm::vec3 & vPosition)
//{
//	return FMOD_VECTOR();
//}
//
//Implementation::Implementation()
//{
//	mpStudioSystem = NULL;
//	AudioManager::errorCheck(FMOD::Studio::System::create(&mpStudioSystem));
//	AudioManager::errorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
//
//	mpSystem = NULL;
//	AudioManager::errorCheck(mpStudioSystem->getCoreSystem(&mpSystem));
//}
//
//Implementation::~Implementation()
//{
//	AudioManager::errorCheck(mpStudioSystem->unloadAll());
//	AudioManager::errorCheck(mpStudioSystem->release());
//}
//
//void Implementation::Update()
//{
//	std::vector<ChannelMap::iterator> pStoppedChannels;
//	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it )
//	{
//		bool bIsPlaying = false;
//		it->second->isPlaying(&bIsPlaying);
//		if (!bIsPlaying)
//		{
//			pStoppedChannels.push_back(it);
//		}
//	}
//
//	for (auto& it : pStoppedChannels)
//	{
//		mChannels.erase(it);
//	}
//	AudioManager::errorCheck(mpStudioSystem->update());
//}
