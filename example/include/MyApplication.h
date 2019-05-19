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

#include "DeferredShader.h"
#include "DeferredLightPassShader.h"
#include "FullScreenShader.h"
#include "ShadowShader.h"
#include "ShadowMapShader.h"
#include "WaterShader.h"
#include "TessShader.h"

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
	void		 sortDraw(glm::mat4& aProjectionView);
	virtual void Destroy();

	unsigned int m_programID;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	glm::vec4 mLightPos;
	glm::vec4 mLightDir;

	Renderer* mRenderer;
	//PostProcessing* mPostProcessing;
	EntityManager* mEntityManager;

	//Models
	Entity* mBeachModel;
	Entity* mChair1;
	Entity* mChair2;
	Entity* mChair3;

	Entity* nanoSuitModel;

	//Lights
	Entity* mAmbientLight;
	Entity* mDirLight;
	Entity* mSpotLight;
	Entity* mPointLight;

	Entity* mModel;
	Entity* mMainCamera;

	Entity* mWaveModel;

	LightVolumeManager* mLightVolumes;

	DeferredShader* mDeferedShader;
	TessShader* mTessShader;
	WaterShader* mWaterShader;
	DeferredLightPassShader* mDeferredLight;
	ShadowShader* mShadowShader;
	ShadowMapShader* mShadowMapShader;
	FullScreenShader* mFullScreenShader;

	GUI* mGUI;

	glm::vec4 cameraMat;
	glm::mat4 mShadowMatrix;
};

#endif // __Application_H_