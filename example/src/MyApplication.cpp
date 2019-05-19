#include "MyApplication.h"
#include "Gizmos.h"
#include "Utilities.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>
#include <memory>
#include <sstream>
#include <string>

#include "ECS.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "MeshComponent.h"

#include "FBXFile.h"

#include "Application_Log.h"

MyApplication::MyApplication()
{
}

MyApplication::~MyApplication()
{
}

bool MyApplication::onCreate()
{
	//start the logger
	Engine::Log::Init();

	EN_CORE_TRACE("EXAMPLE PROJECT INIT");

	mRenderer = DEBUG_NEW Renderer();
	//mPostProcessing = DEBUG_NEW PostProcessing();
	mEntityManager = DEBUG_NEW EntityManager();

	//Setup GUi
	mGUI = DEBUG_NEW GUI();
	mGUI->addGUIElement<GUIHierarchy>(mEntityManager, 
		glm::vec2(m_windowWidth * 0.8f, 0.0f),
		glm::vec2(m_windowWidth * 0.2f, m_windowHeight * 0.2f));

	mGUI->addGUIElement<GUIInspector>(
		glm::vec2(m_windowWidth * 0.8f, m_windowHeight * 0.2f),
		glm::vec2(m_windowWidth * 0.2f, m_windowHeight * 0.3f));

	// initialise the Gizmos helper class
	Gizmos::create();
	glfwSwapInterval(0);

	mLightPos = glm::vec4(50.f, 25.f, 0.f, 1.f);
	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(mLightPos.xyz),
		glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -100, 100);

	//Create a DEBUG_NEW light volumes manager
	mLightVolumes = DEBUG_NEW LightVolumeManager();

	//Setup the main camera
	mMainCamera = mEntityManager->addEntity();
	mMainCamera->setID("Main Camera");
	mMainCamera->addComponent<TransformComponent>();
	mMainCamera->addComponent<CameraComponent>();
	mMainCamera->getComponent<CameraComponent>().setCameraMatrix(glm::inverse(glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))));
	mMainCamera->getComponent<CameraComponent>().setCameraPropertiesPre(0.25f, (float)m_windowWidth / (float)m_windowHeight, 0.1f, 1000.0f);
	//mMainCamera->addComponent<SpotLight>();
	//mMainCamera->getComponent<SpotLight>().setPosition(*mMainCamera->getComponent<TransformComponent>().getPosition());

	GBuffer* gbuffer = DEBUG_NEW GBuffer();
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

	//mPostProcessing->getEffect<PPBlur>()->setFinalImage(*gbuffer->getTexture(5));

	//Setup the deferred pass
	const char* szOutputsGoem[] = { "Diffuse", "Normal", "World", "Specular" };
	mDeferedShader = DEBUG_NEW DeferredShader(
		"./shaders/deferred/vertex_def.glsl",
		"",
		"",
		"",
		"./shaders/deferred/fragment_def.glsl",
		3, nullptr, 3, szOutputsGoem);
	mDeferedShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mDeferedShader->setGBuffer(gbuffer);

	mTessShader = DEBUG_NEW TessShader(
		"./shaders/tess/vertex_tess.glsl",
		"./shaders/tess/vertex_tessCon.glsl",
		"./shaders/tess/vertex_tessEval.glsl",
		"",
		"./shaders/tess/fragment_tess.glsl",
		3, nullptr, 3, szOutputsGoem);
	mTessShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mTessShader->setVec4("CameraPos", *mMainCamera->getComponent<TransformComponent>().getPosition());
	mTessShader->setGBuffer(gbuffer);

	mWaterShader = DEBUG_NEW WaterShader(
		"./shaders/vertexAnim/vertex_water.glsl",
		"",
		"",
		"",
		"./shaders/vertexAnim/fragment_water.glsl"
	);
	mWaterShader->setGBuffer(gbuffer);
	mWaterShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mWaterShader->setVec4("cameraPos", *mMainCamera->getComponent<TransformComponent>().getPosition());
	mWaterShader->mMainCamera = mMainCamera;

	//Setup the deferred light pass
	const char* szInputsFS[] = { "Position" };
	const char* szOutputDefLight[] = { "FragColour", "HDRColour" };
	mDeferredLight = DEBUG_NEW DeferredLightPassShader(
		"./shaders/deferred/vertex_def_light.glsl",
		"",
		"",
		"",
		"./shaders/deferred/fragment_def_light.glsl",
		1, szInputsFS,
		2, szOutputDefLight);
	mDeferredLight->setUnsignedIntUniforms("DiffuseTex", *mDeferedShader->getGBuffer()->getTexture(0));
	mDeferredLight->setUnsignedIntUniforms("NormalTex", *mDeferedShader->getGBuffer()->getTexture(1));
	mDeferredLight->setUnsignedIntUniforms("WPosTex", *mDeferedShader->getGBuffer()->getTexture(2));
	mDeferredLight->setUnsignedIntUniforms("SpecTex", *mDeferedShader->getGBuffer()->getTexture(3));
	mDeferredLight->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mDeferredLight->setVec4("CameraPos", *mMainCamera->getComponent<TransformComponent>().getPosition());
	//Setup the deferred stencil pass
	mDeferredLight->getStencilShader()->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mDeferredLight->setGBuffer(gbuffer);

	mShadowShader = DEBUG_NEW ShadowShader(
		"./shaders/shadows/vertex_shadow.glsl",
		"./shaders/shadows/fragment_shadow.glsl"
	);
	//mShadowShader->setUnsignedIntUniforms("ShadowTexture", *mDeferedShader->getTexture(0));

	mShadowMapShader = DEBUG_NEW ShadowMapShader(
		"./shaders/shadows/vertex_shadow_map.glsl",
		"./shaders/shadows/fragment_shadow_map.glsl"
	);
	mShadowMapShader->setMat4("ProjectionView", *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix());
	mShadowMapShader->setUnsignedIntUniforms("texture_worldPos", *gbuffer->getTexture(2));
	mDeferredLight->setUnsignedIntUniforms("ShadowTex", *mShadowMapShader->getTexture(0));

	mBeachModel = mEntityManager->addEntity();
	mBeachModel->setID("beach Model");
	mBeachModel->addComponent<TransformComponent>();
	mBeachModel->getComponent<TransformComponent>().setPosition(glm::vec4(0, 0, 0, 1));
	mBeachModel->addComponent<MeshComponent>("./models/ruinedtank/tank.fbx", LoadType::AssimpLoader);
	mBeachModel->getComponent<MeshComponent>().addShader(mTessShader);
	mBeachModel->getComponent<MeshComponent>().setRenderMode(RenderMode::Opaque);

	//mChair1 = mEntityManager->addEntity();
	//mChair1->setID("Chair_1 Model");
	//mChair1->addComponent<TransformComponent>();
	//mChair1->getComponent<TransformComponent>().setPosition(glm::vec4(5, 0, 0, 1));
	//mChair1->addComponent<MeshComponent>("./models/chair/testChair.obj", LoadType::AssimpLoader);
	////mChair1->getComponent<MeshComponent>().addShader(mDeferedShader);
	//mChair1->getComponent<MeshComponent>().setRenderMode(RenderMode::Opaque);

	//mChair2 = mEntityManager->addEntity();
	//mChair2->setID("Chair_2 Model");
	//mChair2->addComponent<TransformComponent>();
	//mChair2->getComponent<TransformComponent>().setPosition(glm::vec4(-10, 7, 3, 1));
	//mChair2->addComponent<MeshComponent>("./models/chair/testChair.obj", LoadType::AssimpLoader);
	//mChair2->getComponent<MeshComponent>().addShader(mDeferedShader);
	//mChair2->getComponent<MeshComponent>().setRenderMode(RenderMode::Opaque);

	nanoSuitModel = mEntityManager->addEntity();
	nanoSuitModel->setID("Nano suit Model");
	nanoSuitModel->addComponent<TransformComponent>();
	nanoSuitModel->getComponent<TransformComponent>().setPosition(glm::vec4(-5, 2, 3, 1));
	nanoSuitModel->addComponent<MeshComponent>("./models/nano/nanosuit.obj", LoadType::AssimpLoader);
	nanoSuitModel->getComponent<MeshComponent>().addShader(mDeferedShader);
	nanoSuitModel->getComponent<MeshComponent>().setRenderMode(RenderMode::Opaque);

	//mChair3 = mEntityManager->addEntity();
	//mChair3->setID("Chair_3 Model");
	//mChair3->addComponent<TransformComponent>();
	//mChair3->getComponent<TransformComponent>().setPosition(glm::vec4(-5, 0, 0, 1));
	//mChair3->addComponent<MeshComponent>("./models/chair/testChair.obj", LoadType::AssimpLoader);
	//mChair3->getComponent<MeshComponent>().addShader(mDeferedShader);
	//mChair3->getComponent<MeshComponent>().setRenderMode(RenderMode::Opaque);


	mWaveModel = mEntityManager->addEntity();
	mWaveModel->setID("Wave");
	mWaveModel->addComponent<TransformComponent>();
	mWaveModel->getComponent<TransformComponent>().setPosition(glm::vec4(0, 0.25f, 0, 1));
	mWaveModel->addComponent<MeshComponent>();
	mWaveModel->getComponent<MeshComponent>().setRenderMode(RenderMode::Transparent);
	mWaveModel->getComponent<MeshComponent>().addPlane(150, 150);
	mWaveModel->getComponent<MeshComponent>().addShader(mWaterShader);

	//mModel = mEntityManager->addEntity();
	//mModel->setID("Tank Model");
	//mModel->addComponent<TransformComponent>();
	//mModel->getComponent<TransformComponent>().setPosition(glm::vec4(0, 0, 0, 1));
	//mModel->addComponent<MeshComponent>("./models/ruinedtank/tank.fbx", LoadType::AssimpLoader);
	//mModel->getComponent<MeshComponent>().addShader(mDeferedShader);
	//mModel->getComponent<MeshComponent>().addShader(mShadowShader);
	//mModel->getComponent<MeshComponent>().addShader(mShadowMapShader);
	//mModel->getComponent<MeshComponent>().addShader(mWaterShader);

	const char* szInputsFS2[] = { "Position", "TexCoord1" };
	mFullScreenShader = DEBUG_NEW FullScreenShader(
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
	mFullScreenShader->setMat4("cameraMatrix", *mMainCamera->getComponent<CameraComponent>().getCameraMatrix());

	mSpotLight = mEntityManager->addEntity();
	mSpotLight->setID("Spot Light");
	mSpotLight->addComponent<TransformComponent>();
	mSpotLight->addComponent<SpotLight>();

	mPointLight = mEntityManager->addEntity();
	mPointLight->setID("Point Light");
	mPointLight->addComponent<PointLight>();

	mDirLight = mEntityManager->addEntity();
	mDirLight->setID("Direcational Light");
	mDirLight->addComponent<TransformComponent>();
	mDirLight->addComponent<DirectionalLight>();

	mAmbientLight = mEntityManager->addEntity();
	mAmbientLight->setID("Ambient Light");
	mAmbientLight->addComponent<TransformComponent>();
	mAmbientLight->addComponent<AmbientLight>();

	mLightVolumes->addLightVolume(&mAmbientLight->getComponent<AmbientLight>());
	mLightVolumes->addLightVolume(&mDirLight->getComponent<DirectionalLight>());
	mLightVolumes->addLightVolume(&mPointLight->getComponent<PointLight>());
	mLightVolumes->addLightVolume(&mSpotLight->getComponent<SpotLight>());

	mSpotLight->getComponent<SpotLight>().setPosition(glm::vec4(4, 8.f, 4, 1));
	mSpotLight->getComponent<SpotLight>().setDiffuse(glm::vec4(0, 1, 0, 1));
	mSpotLight->getComponent<SpotLight>().addShadowMap();
	//mShadowShader->addShadow(&mSpotLight->getComponent<SpotLight>());

	mDirLight->getComponent<DirectionalLight>().addShadowMap();
	mDirLight->getComponent<DirectionalLight>().setPosition(mLightPos);
	mDirLight->getComponent<DirectionalLight>().setShadowProViewMatrix(depthProjectionMatrix * depthViewMatrix);
	mShadowShader->addShadow(&mDirLight->getComponent<DirectionalLight>());

	mDeferedShader->setMat4("shadowMatrix", *mDirLight->getComponent<DirectionalLight>().getShadowProViewMatrix());
	mDeferredLight->setLightVolumeManager(mLightVolumes);

	mShadowMapShader->addShadow(&mDirLight->getComponent<DirectionalLight>());

	mRenderer->addShader(mDeferedShader);
	mRenderer->addShader(mTessShader);
	mRenderer->addShader(mWaterShader);
	mRenderer->addShader(mShadowShader);
	mRenderer->addShader(mShadowMapShader);
	mRenderer->addShader(mDeferredLight);
	mRenderer->addShader(mFullScreenShader);

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
	for ( int i = 0 ; i < 21 ; ++i )
	{
		Gizmos::addLine( glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
		
		Gizmos::addLine( glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), 
						 i == 10 ? glm::vec4(1,1,1,1) : glm::vec4(0,0,0,1) );
	}

	//\====================================================
	// ImGui render view to show what is held in the fbo texture position
	//\====================================================
	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - (m_windowWidth * 0.5f), m_windowHeight - (m_windowHeight * 0.5f)));
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f));
	ImGui::Begin("Framebuffer");
	ImGui::BeginTabBar("Framebuffer textures");

	if (ImGui::BeginTabItem("Colour Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(0);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Normal Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(1);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("World Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(2);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Specular Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(3);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(4);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Light Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(5);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("HDR Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDeferedShader->getGBuffer()->getTexture(6);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Dir Light Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mDirLight->getComponent<DirectionalLight>().getShadowMap();
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("mSpotLight Light Depth Buffer"))
	{
		ImTextureID texID = (void*)(intptr_t)*mSpotLight->getComponent<SpotLight>().getShadowMap();
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Shadow Map Master"))
	{
		ImTextureID texID = (void*)(intptr_t)*mShadowMapShader->getTexture(0);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Reflection Master"))
	{
		ImTextureID texID = (void*)(intptr_t)*mWaterShader->mReflectionBuffer.getTexture(0);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Refraction Master"))
	{
		ImTextureID texID = (void*)(intptr_t)*mWaterShader->mRefractionBuffer.getTexture(0);
		ImGui::Image(texID, ImVec2(m_windowWidth * 0.5f, m_windowHeight * 0.5f), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::End();

	static bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);
	Application_Log* log = Application_Log::Get();
	if (log != nullptr && show_demo_window)
	{
		log->showLog(&show_demo_window);
	}
	//show application log window
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS) {
		show_demo_window = !show_demo_window;
	}
	// quit our application when escape is pressed
	if (glfwGetKey(m_window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		quit();
}

void MyApplication::sortDraw(glm::mat4& aProjectionView)
{
	std::vector<Shader*> shaders;
	for (auto& e : mEntityManager->getEntities())
	{
		if (e->hasComponent<MeshComponent>())
		{
			Shader* s = e->getComponent<MeshComponent>().getShader<Shader>();

			if (std::find(shaders.begin(), shaders.end(), s) == shaders.end())
			{
				shaders.push_back(s);

				//bind shader target
				s->useShader(true);
			}
			else
			{
				s->useShader();
			}

			//draw
			e->draw();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glUseProgram(0);
}

void MyApplication::Draw()
{
	// get the view matrix from the world-space camera matrix
	glm::mat4 viewMatrix = mMainCamera->getComponent<CameraComponent>().getViewMatrix();
	glm::mat4 projecMatrix = *mMainCamera->getComponent<CameraComponent>().getProjectionViewMatrix();

	mRenderer->draw();
	//mPostProcessing->draw();
	mGUI->draw();
}

void MyApplication::Destroy()
{
	Gizmos::destroy();

	//delete mBaseShader;
	delete mLightVolumes;
	delete mDeferredLight;
	delete mTessShader;
	delete mFullScreenShader;
	delete mDeferedShader;
	delete mShadowShader;
	delete mShadowMapShader;
	delete mWaterShader;

	//delete GUI 
	delete mGUI;

	delete mRenderer;
	//delete mPostProcessing;

	mEntityManager->destroy();
	delete mEntityManager;
}

