#include "Shader/ShadowMapShader.h"
#include "Renderer.h"
#include "Component/DirectionalLight.h"

ShadowMapShader::ShadowMapShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount,
	const char* aInputs[], unsigned int aOutputCount, const char* aOutputs[]) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
}

ShadowMapShader::~ShadowMapShader()
{
}

void ShadowMapShader::initBuffers()
{
	//create new gbuffer
	mShadowMapBuffer = new GBuffer();
	mShadowMapBuffer->setScreenSize(getRenderer()->getScreenSize());
	mShadowMapBuffer->createFrameBuffer();
	mShadowMapBuffer->attachTextureToFrameBuffer(0, GL_RGBA, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Shadow Map Colour Texture");
	mShadowMapBuffer->attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT, "Shadow Map Depth Texture");
	mShadowMapBuffer->setScreenSize(glm::vec2(getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y));
}

void ShadowMapShader::destroy()
{
	//destroy
	mShadowMapBuffer->destroy();
	delete mShadowMapBuffer;
}

void ShadowMapShader::useShader(bool aClear)
{
	//get all lights
	std::vector<DirectionalLight*> lights = getRenderer()->getEntityManager()->getAllEntityComponents<DirectionalLight>();

	//enable cull, depth test and depth write
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	setVec2("screenSize", *mShadowMapBuffer->getScreenSize());
	mShadowMapBuffer->bindBuffer();
	mShadowMapBuffer->setViewPort();

	bool clear = true;
	//loop over all lights
	for (auto& light : lights)
	{
		//set shadow projection view matrix
		setMat4Override("ShadowMatrx", *light->getShadowProViewMatrix());
		setUnsignedIntUniformsOverride("texture_shadow", *light->getShadowMap());
		//set lightType
		if (light->getLightType() == LightType::Directional)
		{
			float lightType = 0.0f;
			setFloatOverride("LightType", lightType);
		}
		else
		{
			float lightType = 1.0f;
			setFloatOverride("LightType", lightType);
		}
		setVec4("lightDir", *light->getLightDirection());

		//render 
		Shader::useShader(clear, false, light->entity);

		clear = false;
	}
	glUseProgram(0);
	mShadowMapBuffer->unBindBuffer();
	//disable cull, depth test and depth write
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	glViewport(0, 0, getRenderer()->getScreenSize().x, getRenderer()->getScreenSize().y);
}

unsigned int * ShadowMapShader::getShadowTexture()
{
	//get shadow texture
	return mShadowMapBuffer->getTexture(0);
}
