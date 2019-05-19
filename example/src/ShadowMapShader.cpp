#include "ShadowMapShader.h"
#include "Utilities.h"
#include "LightComponent.h"
#include "MeshComponent.h"

ShadowMapShader::ShadowMapShader(const char * aVertexPath, const char * aFragmentPath)
{
	unsigned int vertex = Utility::loadShader(aVertexPath, GL_VERTEX_SHADER);
	unsigned int fragment = Utility::loadShader(aFragmentPath, GL_FRAGMENT_SHADER);
	const char* inputs[] = { "Position", "Normal", "TexCoord1" };
	const char* output[] = { "FragColour" };
	mShaderID = Utility::createProgram(
		vertex,
		0,
		0,
		0,
		fragment,
		3, inputs,
		1, output
	);

	createFrameBuffer();
	attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0);
	attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT);
}

ShadowMapShader::ShadowMapShader(const char * aVertexPath, const char * aControlpath, const char * aEvaluationpath, 
								 const char * aGeometryPath, const char * aFragmentPath, unsigned int aInputCount, 
								 const char * aInputs[], unsigned int aOutputCount, const char * aOutputs[]) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{

}

ShadowMapShader::~ShadowMapShader()
{
}

void ShadowMapShader::useShader(bool aClear)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ONE);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	bool clear = true;
	for (auto& light : mShadowLights)
	{
		setMat4Override("ShadowMatrx", *light->getShadowProViewMatrix());
		setUnsignedIntUniformsOverride("texture_shadow", *light->getShadowMap());
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

		Shader::useShader(clear, false, light->entity);

		clear = false;
	}

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
}

void ShadowMapShader::addShadow(LightComponent* aLight)
{
	mShadowLights.emplace_back(aLight);
}
