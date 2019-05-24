#ifndef __Application_H_
#define __Application_H_

#include "Log.h"

#include "Application.h"
#include <glm/glm.hpp>

#include "Renderer.h"
#include "EntityManager.h"

#include "PostProcessing.h"
#include "PPScreenOverlay.h"
#include "PPBlur.h"
#include "PPBloom.h"

#include "DeferredShader.h"
#include "TessShader.h"
#include "WaterShader.h"
#include "ShadowShader.h"
#include "ShadowMapShader.h"
#include "SSAOShader.h"
#include "SSRShader.h"
#include "DeferredLightPassShader.h"
#include "FullScreenShader.h"

#include "AssimpModel.h"

#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "SpotLight.h"
#include "LightVolumeManager.h"


#include "GBuffer.h"

#include "GUI.h"
#include "GUIHierarchy.h"
#include "GUIInspector.h"

//#include "Renderer.h"

// Derived application class that wraps up all globals neatly
class MyApplication : public Application
{
public:

	MyApplication();
	virtual ~MyApplication();

protected:
	
	virtual bool onCreate();
	virtual void Update(float a_deltaTime);
	virtual void Draw();
	virtual void Destroy();

	unsigned int m_programID;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	glm::vec4 mLightPos;
	glm::vec4 mLightDir;

	//render
	Renderer* mRenderer;
	//post process
	PostProcessing* mPostProcessing;
	//entity manager
	EntityManager* mEntityManager;

	//Models
	Entity* mTankModel;
	//Entity* mChair1;
	//Entity* mChair2;
	//Entity* mChair3;
	Entity* mWaveModel;
	//Entity* nanoSuitModel;
	//Entity* mModel;

	//Lights
	Entity* mAmbientLight;
	Entity* mDirLight;
	Entity* mSpotLight;
	Entity* mPointLight;

	//Cmaera
	Entity* mMainCamera;


	//light volumes manager
	LightVolumeManager* mLightVolumes;

	//Shaders
	DeferredShader* mDeferedShader;
	TessShader* mTessShader;
	WaterShader* mWaterShader;
	ShadowShader* mShadowShader;
	ShadowMapShader* mShadowMapShader;
	SSAOShader* mSSAOShader;
	SSRShader* mSSRShader;
	DeferredLightPassShader* mDeferredLight;
	FullScreenShader* mFullScreenShader;

	//GUI
	GUI* mGUI;
};

#endif // __Application_H_