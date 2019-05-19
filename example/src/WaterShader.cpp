#include "WaterShader.h"
#include "Utilities.h"

WaterShader::WaterShader()
{
}

WaterShader::WaterShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/,
	const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//glm::vec4* waveData = new glm::vec4[1];
	//waveData[0] = glm::vec4(0, 0, 1, 1);

	//glGenTextures(1, &mWaveTezture);
	//glBindTexture(GL_TEXTURE_2D, mWaveTezture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glBindTexture(GL_TEXTURE_2D, mWaveTezture);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, waveData);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//float pixels = 1;
	//setFloat("numOfPixels", pixels);

	//setUnsignedIntUniforms("waveTexture", mWaveTezture);

	//delete[] waveData;

	unsigned int vertexShader = Utility::loadShader("./shaders/water/vertex_ClipPlane.glsl", GL_VERTEX_SHADER);
	unsigned int fragmentShader = Utility::loadShader("./shaders/water/fragment_ClipPlane.glsl", GL_FRAGMENT_SHADER);

	mClipPlaneProgram = Utility::createProgram(
		vertexShader, 
		0,
		0,
		0,
		fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//mReflection = Utility::createTexture(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//mRefraction = Utility::createTexture(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//mDepth = Utility::createTexture(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	mReflectionBuffer = GBuffer();
	mReflectionBuffer.createFrameBuffer();
	mReflectionBuffer.attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Water Reflection Texture");
	mReflectionBuffer.attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT, "Water Reflection Depth Texture");

	mRefractionBuffer = GBuffer();
	mRefractionBuffer.createFrameBuffer();
	mRefractionBuffer.attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Water Refraction Texture");
	mRefractionBuffer.attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT, "Water Refraction Depth Texture");

	mClipPlane = glm::vec4(0, -1, 0, 2);
	setVec4("ClippingPlane", mClipPlane);

	//waveData = new glm::vec4[3];
	waveData[0] = glm::vec4(0, -1, 0.0f, 2);
	waveData[1] = glm::vec4(0, 1, 0.25f, 20);
	waveData[2] = glm::vec4(1, 1, 0.15f, 10);
	setVec4("waveData", waveData[0]);

	mMoveFactor = 0;
}

WaterShader::~WaterShader()
{
	//delete[] waveData;

	glDeleteProgram(mClipPlaneProgram);
	mReflectionBuffer.destroy();
	mRefractionBuffer.destroy();
	glDeleteTextures(1, &mDUDVMap);
	glDeleteTextures(1, &mNormalMap);
}

void WaterShader::useShader(bool aClear)
{
	aClear;

	if (mDUDVMap == 0)
	{
		Utility::loadTextureFromFile("./water_dudvmap.png", mDUDVMap);
		Utility::loadTextureFromFile("./waterNormalMap.png", mNormalMap);
	}

	mMoveFactor += 0.03f * Utility::getDeltaTime();
	if (mMoveFactor > 1)
	{
		mMoveFactor = 0;
	}

	glm::vec4 cameraPos = *mMainCamera->getComponent<TransformComponent>().getPosition();
	float distance = 2 * (cameraPos.y - waveData[0].w);
	*mMainCamera->getComponent<TransformComponent>().getPosition() -= distance;
	mMainCamera->getComponent<CameraComponent>().invertPitch();

	glm::mat4 reflectionMatrix;
	reflectionMatrix = glm::reflect3D(reflectionMatrix, glm::vec3(0, 1, 0));
	glm::vec3 translation = -2.f * glm::dot(glm::vec3(0, 0.25f, 0), glm::vec3(0,1,0)) *  glm::vec3(0, 1, 0);
	glm::mat4 planeCorrectionMatrix = glm::translate(translation);
	glm::mat4 newWorldMatrix = reflectionMatrix * planeCorrectionMatrix;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	mReflectionBuffer.bindBuffer();
	mReflectionBuffer.setDrawBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mClipPlaneProgram);

	waveData[0] = glm::vec4(0, -1, 0.0f, 0.25f);
	setUniforms(mClipPlaneProgram);

	std::vector<MeshComponent*> allMeshes = getMeshComponent()->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() != std::string("Wave"))
		{
			allMeshes[i]->draw(this, mClipPlaneProgram, newWorldMatrix);
		}
	}

	mReflectionBuffer.unBindBuffer();
	glFrontFace(GL_CCW);
	////////////////////////////////////////////////////////////////

	*mMainCamera->getComponent<TransformComponent>().getPosition() += distance;
	mMainCamera->getComponent<CameraComponent>().invertPitch();

	////////////////////////////////////////////////////////////////
	mRefractionBuffer.bindBuffer();
	mRefractionBuffer.setDrawBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	waveData[0] = glm::vec4(0, -1, 0, 0.25f);
	setUniforms(mClipPlaneProgram);

	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() != std::string("Wave"))
		{
			allMeshes[i]->draw(this, mClipPlaneProgram);
		}
	}

	mRefractionBuffer.unBindBuffer();
	glUseProgram(0);

	glDisable(GL_CLIP_DISTANCE0);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	mGBuffer->bindBuffer();
	//mGBuffer->setDrawBuffers();
	glViewport(0, 0, 1920, 1080);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//
	float time = Utility::getTotalTime();
	setFloat("time", time);

	float numOfPixels = 3;
	setFloat("numOfPixels", numOfPixels);
	//glm::vec4 wave = glm::vec4(1, 0, 0.5f, 10);
	//setVec4("waveA", wave);

	//glm::vec4 waveB = glm::vec4(0, 1, 0.25f, 20);
	//setVec4("waveB", waveB);

	//glm::vec4 waveC = glm::vec4(1, 1, 0.15f, 10);
	//setVec4("waveC", waveC);

	setUnsignedIntUniforms("texture_reflection", *mReflectionBuffer.getTexture(0));
	setUnsignedIntUniforms("texture_refraction", *mRefractionBuffer.getTexture(0));
	setUnsignedIntUniforms("texture_DUDVMap", mDUDVMap);
	setUnsignedIntUniforms("texture_normal", mNormalMap);
	setUnsignedIntUniforms("texture_waterDepth", *mRefractionBuffer.getTexture(1));
	setFloat("moveFactor", mMoveFactor);

	glUseProgram(mShaderID);

	setUniforms();

	for (unsigned int i = 0; i < allMeshes.size(); ++i)
	{
		if (allMeshes[i]->entity->getID() == std::string("Wave"))
		{
			allMeshes[i]->draw(this);
		}
	}

	glUseProgram(0);

	//
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 1920, 1080);
	mGBuffer->unBindBuffer();
}
