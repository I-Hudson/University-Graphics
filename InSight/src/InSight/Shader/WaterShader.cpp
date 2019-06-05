#include "Shader/WaterShader.h"
#include "Utilities.h"
#include "Renderer.h"

WaterShader::WaterShader()
{
}

WaterShader::WaterShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/,
	const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//load shaders
	unsigned int vertexShader = Utility::loadShader("./shaders/water/vertex_ClipPlane.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = Utility::loadShader("./shaders/water/fragment_ClipPlane.glsl", GL_FRAGMENT_SHADER);

	//create program
	mClipPlaneProgram = Utility::createProgram(
		vertexShader, 
		0,
		0,
		0,
		fragmentShader);

	//delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//set clip plane
	mClipPlane = glm::vec4(0, -1, 0, 2);
	setVec4("ClippingPlane", mClipPlane);

	//set wave data
	waveData = new glm::vec4[3];
	waveData[0] = glm::vec4(0, -1, 0.0f, 2);
	waveData[1] = glm::vec4(0, 1, 0.25f, 20);
	waveData[2] = glm::vec4(1, 1, 0.15f, 10);
	setVec4("waveData", waveData[0]);

	mMoveFactor = 0;
}

WaterShader::~WaterShader()
{

}

void WaterShader::initBuffers()
{
	//create reflection buffer
	mReflectionBuffer = new GBuffer();
	mReflectionBuffer->setScreenSize(getRenderer()->getScreenSize());
	mReflectionBuffer->createFrameBuffer();
	mReflectionBuffer->attachTextureToFrameBuffer(0, GL_RGBA, mReflectionBuffer->getScreenSize()->x, mReflectionBuffer->getScreenSize()->y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Water Reflection Texture");
	mReflectionBuffer->attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, mReflectionBuffer->getScreenSize()->x, mReflectionBuffer->getScreenSize()->y, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT, "Water Reflection Depth Texture");

	//create refraction buffer
	mRefractionBuffer = new GBuffer();
	mRefractionBuffer->setScreenSize(getRenderer()->getScreenSize());
	mRefractionBuffer->createFrameBuffer();
	mRefractionBuffer->attachTextureToFrameBuffer(0, GL_RGBA, mRefractionBuffer->getScreenSize()->x, mRefractionBuffer->getScreenSize()->y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Water Refraction Texture");
	mRefractionBuffer->attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, mRefractionBuffer->getScreenSize()->x, mRefractionBuffer->getScreenSize()->y, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT, "Water Refraction Depth Texture");
}

void WaterShader::destroy()
{
	//destroy/delete
	delete[] waveData;

	mReflectionBuffer->destroy();
	delete mReflectionBuffer;
	mRefractionBuffer->destroy();
	delete mRefractionBuffer;

	glDeleteProgram(mClipPlaneProgram);
	glDeleteTextures(1, &mDUDVMap);
	glDeleteTextures(1, &mNormalMap);

	Shader::destroy();
}

void WaterShader::useShader(bool aClear)
{
	//if no mesh return
	if (getMeshComponent() == nullptr)
	{
		return;
	}
	aClear;

	//if no dudv map, load dudv map and normal map
	if (mDUDVMap == 0)
	{
		Utility::loadTextureFromFile("./water_dudvmap.png", mDUDVMap);
		Utility::loadTextureFromFile("./waterNormalMap.png", mNormalMap);
	}

	//update move factor
	mMoveFactor += 0.03f * Utility::getDeltaTime();
	if (mMoveFactor > 1)
	{
		mMoveFactor = 0;
	}

	//glm::vec4 cameraPos = *mMainCamera->getComponent<TransformComponent>()->getPosition();
	//float distance = 2 * (cameraPos.y - waveData[0].w);
	//*mMainCamera->getComponent<TransformComponent>()->getPosition() -= distance;
	//mMainCamera->getComponent<CameraComponent>()->invertPitch();

	//reflection matrix
	glm::mat4 reflectionMatrix;
	reflectionMatrix = glm::reflect3D(reflectionMatrix, glm::vec3(0, 1, 0));
	glm::vec3 translation = -2.f * glm::dot(glm::vec3(0, 0.25f, 0), glm::vec3(0,1,0)) *  glm::vec3(0, 1, 0);
	glm::mat4 planeCorrectionMatrix = glm::translate(translation);
	glm::mat4 newWorldMatrix = reflectionMatrix * planeCorrectionMatrix;

	//enable depth test, depth write, clip distance
	//diable blend
	//render clockwise
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//bind buffers
	mReflectionBuffer->bindBuffer();
	mReflectionBuffer->setViewPort();
	mReflectionBuffer->setDrawBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mClipPlaneProgram);

	//set uniforms 
	waveData[0] = glm::vec4(0, -1, 0.0f, 0.25f);
	setUniforms(mClipPlaneProgram);

	//render mesh called Wave
	std::vector<MeshComponent*> allMeshes = getMeshComponent()->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() != std::string("Wave"))
		{
			allMeshes[i]->draw(this, mClipPlaneProgram, newWorldMatrix);
			break;
		}
	}
	//unbind buffer
	mReflectionBuffer->unBindBuffer();
	//render counter clockwise
	glFrontFace(GL_CCW);
	////////////////////////////////////////////////////////////////

	//*mMainCamera->getComponent<TransformComponent>()->getPosition() += distance;
	//mMainCamera->getComponent<CameraComponent>()->invertPitch();

	////////////////////////////////////////////////////////////////
	//bind buffer
	mRefractionBuffer->bindBuffer();
	mRefractionBuffer->setViewPort();
	mRefractionBuffer->setDrawBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set uniforms
	waveData[0] = glm::vec4(0, -1, 0, 0.25f);
	setUniforms(mClipPlaneProgram);

	//render all meshes not called Wave
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() != std::string("Wave"))
		{
			allMeshes[i]->draw(this, mClipPlaneProgram);
		}
	}

	mRefractionBuffer->unBindBuffer();
	glUseProgram(0);

	//disable clip plane distance, depth
	glDisable(GL_CLIP_DISTANCE0);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	//bind buffer
	mGBuffer->bindBuffer();
	//mGBuffer->setDrawBuffers();
	glViewport(0, 0, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set float uniform
	float time = Utility::getTotalTime();
	setFloat("time", time);

	//float numOfPixels = 3;
	//setFloat("numOfPixels", numOfPixels);
	//glm::vec4 wave = glm::vec4(1, 0, 0.5f, 10);
	//setVec4("waveA", wave);

	//glm::vec4 waveB = glm::vec4(0, 1, 0.25f, 20);
	//setVec4("waveB", waveB);

	//glm::vec4 waveC = glm::vec4(1, 1, 0.15f, 10);
	//setVec4("waveC", waveC);

	//set texture uniforms
	setUnsignedIntUniforms("texture_reflection", *mReflectionBuffer->getTexture(0));
	setUnsignedIntUniforms("texture_refraction", *mRefractionBuffer->getTexture(0));
	setUnsignedIntUniforms("texture_DUDVMap", mDUDVMap);
	setUnsignedIntUniforms("texture_normal", mNormalMap);
	setUnsignedIntUniforms("texture_waterDepth", *mRefractionBuffer->getTexture(1));
	setFloat("moveFactor", mMoveFactor);

	glUseProgram(mShaderID);

	setUniforms();

	//render Wave mesh
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() == std::string("Wave"))
		{
			allMeshes[i]->draw(this);
			break;
		}
	}

	glUseProgram(0);

	//disable blend, dpeth
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y);
	mGBuffer->unBindBuffer();
}
