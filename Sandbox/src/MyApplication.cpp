#include "MyApplication.h"
#include "InSight/Gizmos.h"
#include "InSight/Utilities.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>
#include <memory>
#include <sstream>
#include <string>

#include "InSight/Entity/ECS.h"
#include "InSight/Component/TransformComponent.h"
#include "InSight/Component/CameraComponent.h"
#include "InSight/Component/MeshComponent.h"

#include "InSight/Application_Log.h"

#include <InSight/InSight.h>
#include "InSight/Event/ApplicationEvent.h"

class InputLayer : public Layer
{
public:
	InputLayer()
		:Layer("InputLayer")
	{
	}

	void OnUpdate() override
	{
		if (InSight::Input::IsKeyPressed(EN_KEY_TAB))
		{
			EN_TRACE("Tab key was pressed");
		}
	}

	void OnEvent(Event& aE) override
	{
		if (aE.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent e = (KeyPressedEvent&)aE;
			EN_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

MyApplication::MyApplication()
{
}

MyApplication::~MyApplication()
{
}

bool MyApplication::onCreate()
{
	EN_TRACE("Project Init");

	InSight::SceneManager::Get().AddScene("New Scene");

	mBaseRenderer = new BaseRenderer();
	mBaseRenderer->setScreenSize(glm::vec2(mWindow->GetWidth(), mWindow->GetHeight()));
	mPostProcessing = new PostProcessing();
	mEntityManager = new EntityManager();

	mBaseRenderer->setEntityManager(mEntityManager);

	//Setup GUi
	mGUI = new GUI();
	mGUI->addGUIElement<GUIHierarchy>(mEntityManager, 
		glm::vec2(mWindow->GetWidth() * 0.8f, 0.0f),
		glm::vec2(mWindow->GetWidth() * 0.2f, mWindow->GetHeight() * 0.2f));

	mGUI->addGUIElement<GUIInspector>(
		glm::vec2(mWindow->GetWidth() * 0.8f, mWindow->GetHeight() * 0.2f),
		glm::vec2(mWindow->GetWidth() * 0.2f, mWindow->GetHeight() * 0.3f));

	mGUI->addGUIElement<InSight::GUIToolBar>(mEntityManager,
		glm::vec2(0.0f, 0.0f),
		glm::vec2(mWindow->GetWidth(), mWindow->GetHeight() * 0.05f));

	// initialise the Gizmos helper class
	Gizmos::create();

	mLightPos = glm::vec4(50.f, 25.f, 0.f, 1.f);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(mLightPos.xyz),
		glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);

	//Create a new light volumes manager
	mLightVolumes = DEBUG_NEW LightVolumeManager();

	//Setup the main camera
	mMainCamera = mEntityManager->addEntity();
	mMainCamera->setID("Main Camera");
	mMainCamera->addComponent<TransformComponent>();
	mMainCamera->addComponent<CameraComponent>();
	mMainCamera->getComponent<CameraComponent>()->setCameraMatrix(glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))));
	mMainCamera->getComponent<CameraComponent>()->setCameraPropertiesPre(0.25f, (float)mWindow->GetWidth() / (float)mWindow->GetHeight(), 0.1f, 1000.0f);
	//mMainCamera->addComponent<SpotLight>();
	//mMainCamera->getComponent<SpotLight>().setPosition(*mMainCamera->getComponent<TransformComponent>().getPosition());

	//\==============================================================================================
	// DEFERRED GBUFFER BaseShader
	//\==============================================================================================
	GBuffer* gbuffer = new GBuffer();
	gbuffer->createFrameBuffer();
	//albdeo
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Albdeo");
	//normal
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT1, "Normal");
	//world
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA32F, 1920, 1080, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT2, "World Position");
	//specular
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT3, "Specular");
	//depth
	gbuffer->attachTextureToFrameBuffer(0, GL_DEPTH24_STENCIL8, 1920, 1080, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0, GL_DEPTH_STENCIL_ATTACHMENT, "Depth");
	//Light
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT4, "Light");
	//HDR
	gbuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0,
		GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT5, "HDR");
	gbuffer->setScreenSize(glm::vec2(mWindow->GetWidth(), mWindow->GetHeight()));

	mPostProcessing->addEffect<PPBlur>();
	mPostProcessing->getEffect<PPBlur>()->setFinalImage(*gbuffer->getTexture(6));
	mPostProcessing->addEffect<PPBloom>();
	mPostProcessing->getEffect<PPBloom>()->setInputImages(*gbuffer->getTexture(5), *gbuffer->getTexture(6));
	mPostProcessing->getEffect<PPBloom>()->mActive = true;

	//\==============================================================================================
	// DEFERRED BaseShader
	//\==============================================================================================
	//Setup the deferred pass
	const char* szOutputsGoem[] = { "Diffuse", "Normal", "World", "Specular" };
	mDeferedShader = mBaseRenderer->addShader<DeferredShader>(
		"./shaders/deferred/vertex_def.glsl",
		"",
		"",
		"",
		"./shaders/deferred/fragment_def.glsl",
		3, nullptr, 4, szOutputsGoem);
	mDeferedShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mDeferedShader->setGBuffer(gbuffer);

	//\==============================================================================================
	// TESS BaseShader
	//\==============================================================================================
	mTessShader = mBaseRenderer->addShader<TessShader>(
		"./shaders/tess/vertex_tess.glsl",
		"./shaders/tess/vertex_tessCon.glsl",
		"./shaders/tess/vertex_tessEval.glsl",
		"",
		"./shaders/tess/fragment_tess.glsl",
		3, nullptr, 3, szOutputsGoem);
	mTessShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mTessShader->setVec4("CameraPos", *mMainCamera->getComponent<TransformComponent>()->getPosition());
	mTessShader->setGBuffer(gbuffer);

	//\==============================================================================================
	// SCREEN SPACE REFLECTIONS BaseShader
	//\==============================================================================================
	mSSRShader = mBaseRenderer->addShader<SSRShader>(
		"./shaders/ssr/vertex_ssr.glsl",
		"",
		"",
		"",
		"./shaders/ssr/fragment_ssr.glsl"
		);
	mSSRShader->setGBuffer(gbuffer);
	mSSRShader->setUnsignedIntUniforms("texture_colour", *gbuffer->getTexture(0));
	mSSRShader->setUnsignedIntUniforms("texture_wPosition", *gbuffer->getTexture(2));
	mSSRShader->setUnsignedIntUniforms("texture_normal", *gbuffer->getTexture(1));
	mSSRShader->setUnsignedIntUniforms("texture_depth", *gbuffer->getTexture(4));
	mSSRShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mSSRShader->setVec4("CameraPos", *mMainCamera->getComponent<TransformComponent>()->getPosition());

	mSSRShader->setMat4("Projection", *mMainCamera->getComponent<CameraComponent>()->getProjectionMatrix());
	mSSRShader->setMat4("view", *mMainCamera->getComponent<CameraComponent>()->getViewMatrix());
	mSSRShader->setMat4("invProjection", *mMainCamera->getComponent<CameraComponent>()->getInvprojectionMatrix());
	mSSRShader->setMat4("invView", *mMainCamera->getComponent<CameraComponent>()->getInvViewMatrix());

	//\==============================================================================================
	// WATER BaseShader
	//\==============================================================================================
	mWaterShader = mBaseRenderer->addShader<WaterShader>(
		"./shaders/vertexAnim/vertex_water.glsl",
		"",
		"",
		"",
		"./shaders/vertexAnim/fragment_water.glsl"
	);
	mWaterShader->initBuffers();
	mWaterShader->setGBuffer(gbuffer);
	mWaterShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mWaterShader->setVec4("cameraPos", *mMainCamera->getComponent<TransformComponent>()->getPosition());
	mWaterShader->mMainCamera = mMainCamera;

	//\==============================================================================================
	// SHADOW BaseShader
	//\==============================================================================================
	const char* shadowOut[] = { "FragDepth" };
	mShadowShader = mBaseRenderer->addShader<ShadowShader>(
		"./shaders/shadows/vertex_shadow.glsl",
		"",
		"",
		"",
		"./shaders/shadows/fragment_shadow.glsl",
		3, nullptr,
		1, shadowOut
		);
	mShadowShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mShadowShader->setUnsignedIntUniforms("texture_worldPos", *gbuffer->getTexture(2));

	//\==============================================================================================
	// SHADOW MAP BaseShader
	//\==============================================================================================
	mShadowMapShader = mBaseRenderer->addShader<ShadowMapShader>(
		"./shaders/shadows/vertex_shadow_map.glsl",
		"",
		"",
		"",
		"./shaders/shadows/fragment_shadow_map.glsl"
	);
	mShadowMapShader->initBuffers();
	mShadowMapShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mShadowMapShader->setUnsignedIntUniforms("texture_worldPos", *gbuffer->getTexture(2));

	//\==============================================================================================
	// SSAO BaseShader
	//\==============================================================================================
	//const char* ssaoInputs[] = { "Position", "TexCoord1" };
	//mSSAOShader = mBaseRenderer->addShader<SSAOShader>(
	//	"./shaders/ssao/vertex_ssao.glsl",
	//	"",
	//	"",
	//	"",
	//	"./shaders/ssao/fragment_ssao.glsl",
	//	2, ssaoInputs
	//	);
	//mSSAOShader->setUnsignedIntUniforms("texture_PostionMap", *gbuffer->getTexture(2));
	//mSSAOShader->setUnsignedIntUniforms("texture_PostionMap", *gbuffer->getTexture(5));
	//mSSAOShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());

	//\==============================================================================================
	// DEFERRED LIGHT BaseShader
	//\==============================================================================================
	//Setup the deferred light pass
	const char* szInputsFS[] = { "Position" };
	const char* szOutputDefLight[] = { "FragColour", "HDRColour" };
	mDeferredLight = mBaseRenderer->addShader<DeferredLightPassShader>(
		"./shaders/deferred/vertex_def_light.glsl",
		"",
		"",
		"",
		"./shaders/deferred/fragment_def_light.glsl",
		1, szInputsFS,
		2, szOutputDefLight
		);
	mDeferredLight->setUnsignedIntUniforms("DiffuseTex", *mDeferedShader->getGBuffer()->getTexture(0));
	mDeferredLight->setUnsignedIntUniforms("NormalTex", *mDeferedShader->getGBuffer()->getTexture(1));
	mDeferredLight->setUnsignedIntUniforms("WPosTex", *mDeferedShader->getGBuffer()->getTexture(2));
	mDeferredLight->setUnsignedIntUniforms("SpecTex", *mDeferedShader->getGBuffer()->getTexture(3));
	mDeferredLight->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mDeferredLight->setVec4("CameraPos", *mMainCamera->getComponent<TransformComponent>()->getPosition());
	mDeferredLight->setUnsignedIntUniforms("ShadowTex", *mShadowMapShader->getShadowTexture());

	//Setup the deferred stencil pass
	mDeferredLight->getStencilShader()->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix());
	mDeferredLight->setGBuffer(gbuffer);

	//\==============================================================================================
	// TANK MODEL
	//\==============================================================================================
	mTankModel = mEntityManager->addEntity();
	mTankModel->setID("Tank Model");
	mTankModel->addComponent<TransformComponent>();
	mTankModel->getComponent<TransformComponent>()->setPosition(glm::vec4(0, 0, 0, 1));
	mTankModel->addComponent<MeshComponent>("./models/ruinedtank/tank.fbx", LoadType::AssimpLoader);
	mTankModel->getComponent<MeshComponent>()->addShader(mTessShader);
	mTankModel->getComponent<MeshComponent>()->setRenderMode(RenderMode::Opaque);

	//\==============================================================================================
	// CHARACTER MODEL
	//\==============================================================================================
	//nanoSuitModel = mEntityManager->addEntity();
	//nanoSuitModel->setID("Nano suit Model");
	//nanoSuitModel->addComponent<TransformComponent>();
	//nanoSuitModel->getComponent<TransformComponent>()->setPosition(glm::vec4(-5, 2, 3, 1));
	//nanoSuitModel->addComponent<MeshComponent>("./models/nano/nanosuit.obj", LoadType::AssimpLoader);
	//nanoSuitModel->getComponent<MeshComponent>()->addShader(mDeferedShader);
	//nanoSuitModel->getComponent<MeshComponent>()->setRenderMode(RenderMode::Opaque);

	//\==============================================================================================
	// WATER MODEL BaseShader
	//\==============================================================================================
	mWaveModel = mEntityManager->addEntity();
	mWaveModel->setID("Wave");
	mWaveModel->addComponent<TransformComponent>();
	mWaveModel->getComponent<TransformComponent>()->setPosition(glm::vec4(0, 0.25f, 0, 1));
	mWaveModel->addComponent<MeshComponent>();
	mWaveModel->getComponent<MeshComponent>()->setRenderMode(RenderMode::Transparent);
	mWaveModel->getComponent<MeshComponent>()->addPlane(100, 100);
	mWaveModel->getComponent<MeshComponent>()->addShader(mWaterShader);
	mSSRShader->setMeshComponent(mWaveModel->getComponent<MeshComponent>());

	//\==============================================================================================
	// FULLSCREEN BaseShader
	//\==============================================================================================
	const char* szInputsFS2[] = { "Position", "TexCoord1" };
	mFullScreenShader = mBaseRenderer->addShader<FullScreenShader>(
		"./shaders/vertex_fs.glsl",
		"",
		"",
		"",
		"./shaders/fragment_fs.glsl",
		2,
		szInputsFS2,
		1,
		nullptr
	);
	mFullScreenShader->setUnsignedIntUniforms("DiffuseTex", *mDeferedShader->getGBuffer()->getTexture(0));
	mFullScreenShader->setUnsignedIntUniforms("NormalTex", *mDeferedShader->getGBuffer()->getTexture(1));
	mFullScreenShader->setUnsignedIntUniforms("WPosTex", *mDeferedShader->getGBuffer()->getTexture(2));
	mFullScreenShader->setUnsignedIntUniforms("FinalTex", *mDeferedShader->getGBuffer()->getTexture(5));
	//mFullScreenShader->setMat4("ShadowProjectionView", *mLightVolumes->getLightVolume<mSpotLight>("Spot_Light_1")->getShadowProViewMatrix());
	mFullScreenShader->setVec4("lightDir", mLightDir);
	mFullScreenShader->setMat4("cameraMatrix", *mMainCamera->getComponent<CameraComponent>()->getCameraMatrix());

	//\==============================================================================================
	// SPOT LIGHT
	//\=============================================================================================
	mSpotLight = mEntityManager->addEntity();
	mSpotLight->setID("Spot Light");
	mSpotLight->addComponent<TransformComponent>();
	mSpotLight->addComponent<SpotLight>();

	//\==============================================================================================
	// POINT LIGHT 
	//\==============================================================================================
	mPointLight = mEntityManager->addEntity();
	mPointLight->setID("Point Light");
	mPointLight->addComponent<PointLight>();
	mPointLight->getComponent<TransformComponent>()->setPosition(glm::vec4(-5.0f, 2.5f, -2.0f, 1.0f));

	//\==============================================================================================
	// DIRECTIONAL LIGHT
	//\==============================================================================================
	mDirLight = mEntityManager->addEntity();
	mDirLight->setID("Direcational Light");
	mDirLight->addComponent<TransformComponent>();
	mDirLight->addComponent<DirectionalLight>();

	//\==============================================================================================
	// AMBIENT LIGHT
	//\==============================================================================================
	mAmbientLight = mEntityManager->addEntity();
	mAmbientLight->setID("Ambient Light");
	mAmbientLight->addComponent<TransformComponent>();
	mAmbientLight->addComponent<AmbientLight>();

	//\==============================================================================================
	// ADD ALL LIGHTS TO LIGHT VOLUME
	//\==============================================================================================
	mLightVolumes->addLightVolume(mAmbientLight->getComponent<AmbientLight>());
	mLightVolumes->addLightVolume(mDirLight->getComponent<DirectionalLight>());
	mLightVolumes->addLightVolume(mPointLight->getComponent<PointLight>());
	mLightVolumes->addLightVolume(mSpotLight->getComponent<SpotLight>());
	mLightVolumes->mEntityManager = mEntityManager;

	//\==============================================================================================
	// SET SPOT LIGHT VALUES
	//\==============================================================================================
	mSpotLight->getComponent<SpotLight>()->setPosition(glm::vec4(11.0f, 8.5f, 10.0f, 1.0f));
	mSpotLight->getComponent<SpotLight>()->setDiffuse(glm::vec4(0, 1, 0, 1));
	mSpotLight->getComponent<SpotLight>()->addShadowMap();
	//mShadowShader->addShadow(&mSpotLight->getComponent<SpotLight>());

	//\==============================================================================================
	// SET DIRECTIONAL LGIHT VALUES
	//\==============================================================================================
	mDirLight->getComponent<DirectionalLight>()->addShadowMap();
	mDirLight->getComponent<DirectionalLight>()->setPosition(mLightPos);
	mDirLight->getComponent<DirectionalLight>()->setShadowProViewMatrix(depthProjectionMatrix * depthViewMatrix);
	//mShadowShader->addShadow(mDirLight->getComponent<DirectionalLight>());

	//\==============================================================================================
	// SET SHADOW MATRIX
	//\==============================================================================================
	mDeferedShader->setMat4("shadowMatrix", *mDirLight->getComponent<DirectionalLight>()->getShadowProViewMatrix());
	mDeferredLight->setLightVolumeManager(mLightVolumes);

	//\==============================================================================================
	// TESS PATHCES
	//\==============================================================================================
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	// set the clear colour and enable depth testing and backface culling
	glClearColor(0.25f, 0.25f, 0.25f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void MyApplication::Update(float a_deltaTime)
{
	// clear all gizmos from last frame
	Gizmos::clear();

	float s = sinf(a_deltaTime * 0.2f);
	float c = cosf(a_deltaTime * 0.2f);
	glm::vec4 temp = mLightPos;
	mLightPos.x = temp.x * c - temp.z * s;
	mLightPos.z = temp.x * s + temp.z * c;
	mLightPos = glm::normalize(mLightPos) * 25.f;
	mLightPos = glm::normalize(mLightPos) * 25.f;
	//Gizmos::addBox(mLightPos.xyz, glm::vec3(0.2f, 0.2f, 0.2f), true, glm::vec4(1.f, 0.85f, 0.05f, 1.f));
	mLightDir = glm::normalize(glm::vec4(0.f, 0.f, 0.f, 1.f) - mLightPos);

	mEntityManager->update();
	mLightVolumes->update();
	mGUI->update();

	// add an identity matrix gizmo
	Gizmos::addTransform( glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1) );
	// add a 20x20 grid on the XZ-plane
	//for ( int i = 0 ; i < 21 ; ++i )
	//{
	//	Gizmos::addLine( glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), 
	//					 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
	//	
	//	Gizmos::addLine( glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), 
	//					 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
	//}

	//\====================================================
	// ImGui render view to show what is held in the fbo texture position
	//\====================================================
	ImGui::SetNextWindowPos(ImVec2(mWindow->GetWidth() - (mWindow->GetWidth() * 0.5f), mWindow->GetHeight() - (mWindow->GetHeight() * 0.5f)));
	ImGui::SetNextWindowSize(ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f));
	ImGui::Begin("Framebuffers");
	ImGui::SliderFloat("Exp", &mPostProcessing->mExposure, 0.1f, 15.0f);
	ImGui::SliderFloat("Gamma", &mPostProcessing->mGamma, 0.1f, 15.0f);
	
	ImGui::BeginTabBar("Framebuffer textures");
	if (ImGui::BeginTabItem("Colour Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(0);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Normal Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(1);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("World Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(2);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(3);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(4);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Light Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(5);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("HDR Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(6);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Dir Light Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDirLight->getComponent<DirectionalLight>()->getShadowMap();
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("mSpotLight Light Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mSpotLight->getComponent<SpotLight>()->getShadowMap();
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Reflection Master"))
	{
		ImTextureID texID = (void*)(intptr_t)*mWaterShader->mReflectionBuffer->getTexture(0);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Refraction Master"))
	{
		ImTextureID texID = (void*)(intptr_t)*mWaterShader->mRefractionBuffer->getTexture(0);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("SSR"))
	{
		ImTextureID texID = (void*)(intptr_t)*mSSRShader->mSSRBuffer->getTexture(0);
		ImGui::Image(texID, ImVec2(mWindow->GetWidth() * 0.5f, mWindow->GetHeight() * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();

	static bool show_demo_window = false;
	//ImGui::ShowDemoWindow(&show_demo_window);
	Application_Log* log = Application_Log::Get();
	if (log != nullptr && show_demo_window)
	{
		log->showLog(&show_demo_window);
	}
	//show application log window
	if (InSight::Input::IsKeyPressed(EN_KEY_LEFT_CONTROL) && InSight::Input::IsKeyPressed(EN_KEY_L))
	{
		show_demo_window = !show_demo_window;
	}
	// quit our application when escape is pressed
	if (InSight::Input::IsKeyPressed(EN_KEY_ESCAPE))
	{
		quit();
	}
}

//void MyApplication::sortDraw(glm::mat4& aProjectionView)
//{
//	std::vector<BaseShader*> shaders;
//	for (auto& e : mEntityManager->getEntities())
//	{
//		if (e->hasComponent<MeshComponent>())
//		{
//			BaseShader* s = e->getComponent<MeshComponent>().getShader<BaseShader>();
//
//			if (std::find(shaders.begin(), shaders.end(), s) == shaders.end())
//			{
//				shaders.push_back(s);
//
//				//bind BaseShader target
//				s->useShader(true);
//			}
//			else
//			{
//				s->useShader();
//			}
//
//			//draw
//			e->draw();
//		}
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	}
//	glUseProgram(0);
//}

void MyApplication::Draw()
{
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = *mMainCamera->getComponent<CameraComponent>()->getViewMatrix();
	glm::mat4 projecMatrix = *mMainCamera->getComponent<CameraComponent>()->getProjectionViewMatrix();

	//\==============================================================================================
	// RENDER
	//\==============================================================================================
	mBaseRenderer->draw();
	mPostProcessing->draw();
	mGUI->draw();
}

void MyApplication::Destroy()
{
	Gizmos::destroy();

	delete mLightVolumes;

	mBaseRenderer->destroy();
	delete mBaseRenderer;

	mPostProcessing->destroy();
	delete mPostProcessing;

	InSight::SceneManager::Get().Destroy();

	mEntityManager->destroy();
	delete mEntityManager;

	//delete GUI 
	delete mGUI;
}

