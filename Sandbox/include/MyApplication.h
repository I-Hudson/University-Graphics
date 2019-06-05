#ifndef __Application_H_
#define __Application_H_

#include "InSight/Application.h"
#include <glm/glm.hpp>

#include "InSight/Log.h"

#include "InSight/Renderer.h"
#include "InSight/Entity/EntityManager.h"

#include "InSight/PP/PostProcessing.h"
#include "InSight/PP/PPScreenOverlay.h"
#include "InSight/PP/PPBlur.h"
#include "InSight/PP/PPBloom.h"

#include "InSight/Shader/DeferredShader.h"
#include "InSight/Shader/TessShader.h"
#include "InSight/Shader/WaterShader.h"
#include "InSight/Shader/ShadowShader.h"
#include "InSight/Shader/ShadowMapShader.h"
#include "InSight/Shader/SSAOShader.h"
#include "InSight/Shader/SSRShader.h"
#include "InSight/Shader/DeferredLightPassShader.h"
#include "InSight/Shader/FullScreenShader.h"

#include "InSight/Assimp/AssimpModel.h"

#include "InSight/Component/PointLight.h"
#include "InSight/Component/DirectionalLight.h"
#include "InSight/Component/AmbientLight.h"
#include "InSight/Component/SpotLight.h"
#include "InSight/LightVolumeManager.h"

#include "InSight/GBuffer.h"

#include "InSight/GUI/GUI.h"
#include "InSight/GUI/GUIHierarchy.h"
#include "InSight/GUI/GUIInspector.h"

#include "Layer/LayerStack.h";

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