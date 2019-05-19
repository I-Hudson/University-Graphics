#include "PostProcessing.h"
#include "PPEffect.h"

PostProcessing::PostProcessing()
{
	generateBuffers();
}

PostProcessing::~PostProcessing()
{
	for (auto& effect : mEffects)
	{
		effect->destory();
		delete effect;
	}
	mEffects.clear();

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
}

std::vector<PPEffect*>* PostProcessing::getAllEffects()
{
	return &mEffects;
}

void PostProcessing::draw()
{
	for (auto& effect : mEffects)
	{
		effect->draw();
	}
}

void PostProcessing::renderQuad()
{
	//Draw Mesh
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/*Private Function*/
void PostProcessing::generateBuffers()
{
	FSVertex mVertices[4];
	mVertices[0] = FSVertex(glm::vec4(-1.f, -1.f, 0.f, 1.f), glm::vec2(0.f, 0.f));
	mVertices[1] = FSVertex(glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec2(1.f, 0.f));
	mVertices[2] = FSVertex(glm::vec4(-1.f, 1.f, 0.f, 1.f), glm::vec2(0.f, 1.f));
	mVertices[3] = FSVertex(glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 1.f));

	unsigned int indics[6] = { 0,1,2,1,3,2 };


	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(FSVertex), mVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indics, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //Pos
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FSVertex), ((char *)0) + 0);

	glEnableVertexAttribArray(1); //Tex1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(FSVertex), ((char *)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
