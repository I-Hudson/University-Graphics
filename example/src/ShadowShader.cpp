#include "ShadowShader.h"
#include "Utilities.h"
#include "LightComponent.h"
#include <glm/gtc/type_ptr.hpp>
#include "MeshComponent.h"

ShadowShader::ShadowShader()
{
}

ShadowShader::ShadowShader(const char* aVertexPath, const char* aFragmentPath)
{
	unsigned int vertex = Utility::loadShader(aVertexPath, GL_VERTEX_SHADER);
	unsigned int fragment = Utility::loadShader(aFragmentPath, GL_FRAGMENT_SHADER);

	const char* szInputs[] = { "Position", "Normal", "TexCoord1" };
	const char* szOutputs[] = { "FragDepth" };

	mShaderID = Utility::createProgram(
		vertex,
		0,
		0,
		0,
		fragment,
		3,
		szInputs,
		1,
		szOutputs);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	//unsigned int fs_vertex = Utility::loadShader("./shaders/shadows/vertex_shadow_map.glsl", GL_VERTEX_SHADER);
	//unsigned int fs_fragment = Utility::loadShader("./shaders/shadows/fragment_shadow_map.glsl", GL_FRAGMENT_SHADER);
	//const char* shadowFSInputs[] = { "Position", "Normal", "TexCoord1" };
	//const char* shadowFSOutput[] = { "FragColour" };
	//mFSShadowShader = Utility::createProgram(
	//	fs_vertex,
	//	0,
	//	0,
	//	0,
	//	fs_fragment,
	//	3,
	//	shadowFSInputs,
	//	1,
	//	shadowFSOutput);
	//glDeleteShader(vertex);
	//glDeleteShader(fragment);

	//createFrameBuffer();
	//attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0);
	//attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT);
}

ShadowShader::ShadowShader(const char * aVertexPath, const char * aControlpath, const char * aEvaluationpath, 
						   const char * aGeometryPath, const char * aFragmentPath, unsigned int aInputCount,
						   const char * aInputs[], unsigned int aOutputCount, const char * aOutputs[]) : 
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//createFrameBuffer();
	//attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0);
	//attachTextureToFrameBuffer(0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0, GL_DEPTH_ATTACHMENT);
}

ShadowShader::~ShadowShader()
{
}

void ShadowShader::useShader(bool aClear)
{
	aClear;
	for (auto& light : mShadowLights)
	{
		light->bindShadowFB();
		
		glUseProgram(mShaderID);
		
		light->bindShadowProViewMatrix(mShaderID);
		
		setUniforms();
		
		std::vector<MeshComponent*> allMeshes = light->entity->getEntityManger()->getAllEntityComponents<MeshComponent>();
		drawMeshes(allMeshes);

		light->unbindShadowFB();

		//blend light output depth to 
		//colour texture from this shader
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glDepthMask(GL_TRUE);
		//glViewport(0, 0, 1920, 1080);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glUseProgram(mFSShadowShader);

		////light->bindShadowProViewMatrix(mFSShadowShader);

		//testMatrix = glm::mat4(
		//	0.0f, -0.0223606806f, -0.00894427206f, 0.0f,
		//	0.0f, 0.0447213612f, -0.00447213603f, 0.0f,
		//	-0.0500000007f, 0.0f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 0.559017003f, 1.0f
		//);

		//glActiveTexture(GL_TEXTURE0);
		//unsigned int loc = glGetUniformLocation(mFSShadowShader, "ShadowTexture");
		//glUniform1i(loc, 0);
		//glBindTexture(GL_TEXTURE_2D, *light->getShadowMap());

		//setUniforms(mFSShadowShader);

		////unsigned int loc = glGetUniformLocation(mFSShadowShader, "ShadowMatrix");
		////glUniformMatrix4fv(loc, 1, false, glm::value_ptr(testMatrix));

		//unsigned int loc1 = glGetUniformLocation(mFSShadowShader, "ProjectionView");
		//glm::mat4 v;
		//glGetUniformfv(mFSShadowShader, loc1, glm::value_ptr(v));

		//drawMeshes(false);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glUseProgram(0);

		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		//glDepthMask(GL_FALSE);
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE);
	}
	glUseProgram(0);
}

void ShadowShader::addShadow(LightComponent* aLight)
{
	mShadowLights.emplace_back(aLight);
}
