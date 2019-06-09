#include "Shader/SSAOShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include "Utilities.h"

SSAOShader::SSAOShader()
{
}

SSAOShader::SSAOShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/,
	const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) :
	BaseShader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//create view normal BaseShader
	const char* pressaoInputs[] = { "Position", "Normal" };
	mViewNormalShader = new BaseShader(
		"./shaders/ssao/vertex_preSSAO.glsl",
		"",
		"",
		"",
		"./shaders/ssao/fragment_preSSAO.glsl",
		2, pressaoInputs
	);

	//create blur BaseShader
	const char* ssaoInputs[] = { "Position", "TexCoord1" };
	mBlurShader = new BaseShader(
		"./shaders/pp/pp_gaussianBlurVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_gaussianBlurFragment.glsl",
		2, ssaoInputs
	);

	//create framebuffer
	mSSAOBuffer.createFrameBuffer();
	mSSAOBuffer.attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Colour Eval Blur");

	//gnereate quad
	generateQuad();

	//mKernel.reserve(64);
	//std::uniform_real_distribution<double> randomFloats(0.0, 1.0);
	//std::default_random_engine generator;
	//for (int i = 0; i < 64; i++)
	//{
	//	glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(randomFloats));
	//	sample = glm::normalize(sample);
	//	sample *= randomFloats(generator);

	//	float scale = float(i) / 64.0f;

	//	scale = lerp(0.1f, 1.0f, scale * scale);
	//	sample *= scale;
	//	mKernel.push_back(sample);
	//}

	//load texture from file
	Utility::loadTextureFromFile("ssaoNoise.jpg", mNoiseTexture);
	setUnsignedIntUniforms("randomMapTexture", mNoiseTexture);
}

SSAOShader::~SSAOShader()
{
}

void SSAOShader::destroy()
{
	//destroy/delete
	mKernel.clear();
	glDeleteTextures(1, &mNoiseTexture);
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);

	mViewNormalShader->destroy();
	delete mViewNormalShader;

	mSSAOBuffer.destroy();
	mBlurShader->destroy();
	delete mBlurShader;

	BaseShader::destroy();
}

void SSAOShader::useShader(bool aClear)
{
	//bind buffer
	mSSAOBuffer.bindBuffer();
	BaseShader::useShaderPP(true);

	//set kernal uniform
	//unsigned int loc = glGetUniformLocation(mShaderID, "gKernel");
	//glUniform3fv(loc, 1, glm::value_ptr(*mKernel));

	//render 
	renderQuad();
	glUseProgram(0);
	mSSAOBuffer.unBindBuffer();
}

void SSAOShader::renderQuad()
{
	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SSAOShader::generateQuad()
{
	//full screen vertices
	FSVertex mVertices[4];
	mVertices[0] = FSVertex(glm::vec4(-1.f, -1.f, 0.f, 1.f), glm::vec2(0.f, 0.f));
	mVertices[1] = FSVertex(glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec2(1.f, 0.f));
	mVertices[2] = FSVertex(glm::vec4(-1.f, 1.f, 0.f, 1.f), glm::vec2(0.f, 1.f));
	mVertices[3] = FSVertex(glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 1.f));

	unsigned int indics[6] = { 0,1,2,1,3,2 };

	//gen buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);

	//bind buffer data
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(FSVertex), mVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indics, GL_STATIC_DRAW);

	//set vertex attributes
	glEnableVertexAttribArray(0); //Pos
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FSVertex), ((char *)0) + 0);

	glEnableVertexAttribArray(1); //Tex1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(FSVertex), ((char *)0) + 16);

	glBindVertexArray(0);
}
