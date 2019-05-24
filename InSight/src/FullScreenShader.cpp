#include "FullScreenShader.h"
#include "Renderer.h"

FullScreenShader::FullScreenShader()
{
}

FullScreenShader::FullScreenShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/,
	const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//full screen vertices
	FullScreenVertex vertexData[4];
	vertexData[0] = FullScreenVertex(glm::vec4(-1.f, -1.f, 0.f, 1.f), glm::vec2(0.f, 0.f));
	vertexData[1] = FullScreenVertex(glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec2(1.f, 0.f));
	vertexData[2] = FullScreenVertex(glm::vec4(-1.f, 1.f, 0.f, 1.f), glm::vec2(0.f, 1.f));
	vertexData[3] = FullScreenVertex(glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 1.f));

	unsigned int indics[6] = { 0,1,2,1,3,2 };

	//gen buffers for full screen quad
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(FullScreenVertex), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indics, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Pos
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FullScreenVertex), ((char *)0) + 0);

	glEnableVertexAttribArray(1); //Tex1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(FullScreenVertex), ((char *)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FullScreenShader::~FullScreenShader()
{
	//delete/destroy
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
}

void FullScreenShader::useShader(bool aClear)
{
	aClear;

	setVec2("screenSize", glm::vec2(getRenderer()->getScreenSize()));

	//bind back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 
	//use program
	glUseProgram(mShaderID);

	//bind vao
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	
	//set uniforms
	setUniforms();

	//render quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
