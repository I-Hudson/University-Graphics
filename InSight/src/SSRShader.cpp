#include "SSRShader.h"
#include "CameraComponent.h"
#include "Renderer.h"
#include "MeshComponent.h"

SSRShader::SSRShader()
{
}

SSRShader::SSRShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
	const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount /*= 3*/,
	const char* aInputs[] /*= nullptr*/, unsigned int aOutputCount /*= 1*/, const char* aOutputs[] /*= nullptr*/) :
	Shader(aVertexPath, aControlpath, aEvaluationpath, aGeometryPath, aFragmentPath, aInputCount, aInputs, aOutputCount, aOutputs)
{
	//generate buffers
	generateBuffers();

	//create gbuffer
	mSSRBuffer = new GBuffer();
	mSSRBuffer->createFrameBuffer();
	mSSRBuffer->attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0,
		GL_COLOR_ATTACHMENT0, "SSR Texture");
}

SSRShader::~SSRShader()
{

}

void SSRShader::useShader(bool aClear)
{
	//if no mesh component return 
	if (getMeshComponent() == nullptr)
	{
		return;
	}

	//enable depth
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//bind buffer
	mSSRBuffer->bindBuffer();
	CameraComponent* camera = getRenderer()->getEntityManager()->getEntity("Main Camera")->getComponent<CameraComponent>();
	MeshComponent* mesh = getMeshComponent();

	Shader::useShaderPP(true);

	//normal matrix
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(*camera->getViewMatrix() * glm::mat4()));
	setMat4("normalMatrix", normalMatrix, true);
	setMat4("invModel", glm::inverse(*mesh->entity->getComponent<TransformComponent>()->getTransformMatrix()), true);

	//draw mesh
	mesh->draw(this);

	glUseProgram(0);
	mSSRBuffer->unBindBuffer();
	//diable depth
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

void SSRShader::destroy()
{
	//destroy/delete
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);

	mSSRBuffer->destroy();
	delete mSSRBuffer;

	Shader::destroy();
}

void SSRShader::renderQuad()
{	
	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SSRShader::generateBuffers()
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
