#include "PP/PostProcessing.h"
#include "PP/PPEffect.h"
#include "imgui.h"

PostProcessing::PostProcessing()
{
	//generate buffers
	generateBuffers();
	mExposure = 0.8f;
	mGamma = 0.5f;
}

PostProcessing::~PostProcessing()
{

}

std::vector<PPEffect*>* PostProcessing::getAllEffects()
{
	//return pointer to vector with all effects
	return &mEffects;
}

void PostProcessing::gui()
{
	//set window pos and size
	ImGui::SetNextWindowPos(ImVec2(0.0f, 100.0f));
	ImGui::SetNextWindowSize(ImVec2(250.0f, 100.0f));

	//gui for post processing
	ImGui::Begin("Post Processing");

	const char* ppEffects[2] = { "PP Blur", "PP Bloom" };
	if(ImGui::ListBox("PP Effect", &mPPEffectActive, ppEffects, 2))
	{
		for (auto& effect : mEffects)
		{
			effect->mActive = false;
		}

		mEffects[mPPEffectActive]->mActive = true;
	}

	ImGui::End();

}

void PostProcessing::destroy()
{
	//destroy all effects 
	for (auto& effect : mEffects)
	{
		effect->destroy();
		delete effect;
	}
	mEffects.clear();

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
}

void PostProcessing::draw()
{
	gui();

	//draw effects
	for (auto& effect : mEffects)
	{
		if (effect->mActive)
		{
			effect->draw();
		}
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

	//gen buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);

	//bind data to buffers
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
