//#pragma once
//
//#ifndef __AUDIO_MANAGER_H__
//#define __AUDIO_MANAGER_H__
//
//#include "glm/glm.hpp"
//#include <string>
//#include <map>
//#include <vector>
//#include <math.h>
//#include <iostream>
//
////fmod includes
//#include "fmod_studio.hpp"
//#include "fmod.hpp"
//
//struct Implementation {
//	Implementation();
//	~Implementation();
//
//	void Update();
//
//	FMOD::Studio::System* mpStudioSystem;
//	FMOD::System* mpSystem;
//
//	int mnNextChannelId;
//
//	typedef std::map<std::string, FMOD::Sound*> SoundMap;
//	typedef std::map<int, FMOD::Channel*> ChannelMap;
//	typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
//	typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;
//
//	BankMap mBanks;
//	EventMap mEvents;
//	SoundMap mSounds;
//	ChannelMap mChannels;
//};
//
//class AudioManager
//{
//public:
//	AudioManager();
//	~AudioManager();
//
//	static void		init();
//	static void		update();
//	static void		destroy();
//	static int 		errorCheck(FMOD_RESULT aResult);
//
//	void			loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
//	void			loadEvent(const std::string& strEventName);
//	void			loadsound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
//	void			unLoadSound(const std::string& strSoundName);
//	void			set3dListenerAndOrientation(const glm::vec3& vPos = glm::vec3(0, 0, 0 ), float fVolumedB = 0.0f);
//	int				playSounds(const std::string &strSoundName, const glm::vec3& vPos = glm::vec3(0, 0, 0), float fVolumedB = 0.0f);
//	void			playEvent(const std::string& strEventName);
//	void			stopChannel(int nChannelId);
//	void			stopEvent(const std::string& strEventName, bool bImmediate = false);
//	void			geteventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
//	void			setEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
//	void			stopAllChannels();
//	void			setChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
//	void			setChannelvolume(int nChannelId, float fVolumedB);
//	bool			isPlaying(int nChannelId) const;
//	bool			isEventPlaying(const std::string& strEventName) const;
//	float			dbToVolume(float db);
//	float			volumeTodb(float volume);
//	FMOD_VECTOR		vectorToFmod(const glm::vec3& vPosition);
//
//private:
//};
//
//#endif // __AUDIO_MANAGER_H__
