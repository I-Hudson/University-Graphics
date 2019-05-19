#include "LightComponent.h"
#include "Utilities.h"
#include "Shader.h"
#include "TransformComponent.h"
#include <glm/gtc/type_ptr.hpp> 

LightComponent::LightComponent()
{
}

LightComponent::~LightComponent()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	//glDeleteBuffers(1, &mIBO);

	delete[] mLightTri;

	if (mShadowsAdded)
	{
		glDeleteBuffers(1, &mShadowFB);
		glDeleteTextures(1, &mShadowTexture);
	}
}

void LightComponent::init()
{
	if (entity->hasComponent<TransformComponent>())
	{
		mTransform = &entity->getComponent<TransformComponent>();
	}
	else
	{
		entity->addComponent<TransformComponent>();
		mTransform = &entity->getComponent<TransformComponent>();
	}

	generateBuffers();

	mLightTri = new  LightTri[mBUFFER_SIZE];
	mLightTriCount = 0;

	mIntensity = 1.0f;

	mShadowsAdded = false;
}

void LightComponent::draw(Shader* aShader /*= nullptr*/, const bool& aBindTextures)
{
	aShader;
	aBindTextures;

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mLightTriCount * sizeof(LightTri), mLightTri);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(1, mIBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size(), mIndices.data(), GL_STATIC_DRAW);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mLightTriCount * 3);
	glBindVertexArray(0);
}

void LightComponent::gui()
{
	if (ImGui::CollapsingHeader("Light Component"))
	{

	}
}

void LightComponent::setPosition(const glm::vec4 aPosition)
{
	mTransform->setPosition(aPosition);
}

void LightComponent::setDiffuse(const glm::vec4& aDiffuse)
{
	mDiffuse = aDiffuse;
}

void LightComponent::setSpecular(const glm::vec4 & aSpecular)
{
	mSpecular = aSpecular;
}

void LightComponent::setIntensity(const float & aIntensity)
{
	mIntensity = aIntensity;
}

glm::vec4* LightComponent::getPosition()
{
	return mTransform->getPosition();
}

glm::vec4* LightComponent::getDiffuse()
{
	return &mDiffuse;
}

glm::vec4* LightComponent::getSpecular()
{
	return &mSpecular;
}

float* LightComponent::getIntensity()
{
	return &mIntensity;
}

glm::vec4* LightComponent::getTargetPosition()
{
	return &mTargetPosition;
}

glm::vec4* LightComponent::getLightDirection()
{
	return &mLightDir;
}

void LightComponent::addShadowMap()
{
	mShadowsAdded = true;
	glGenFramebuffers(1, &mShadowFB);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFB);

	glGenTextures(1, &mShadowTexture);
	glBindTexture(GL_TEXTURE_2D, mShadowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mShadowTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{

		printf("Framebuffer Error\n");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int* LightComponent::getShadowMap()
{
	return &mShadowTexture;
}

void LightComponent::bindShadowFB()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1024, 1024);
}

void LightComponent::unbindShadowFB()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glViewport(0, 0, 1920, 1080);
}

glm::mat4* LightComponent::getShadowProViewMatrix()
{
	return &mShadowProViewMatrix;
}

void LightComponent::setShadowProViewMatrix(const glm::mat4 & aMat4)
{
	mShadowProViewMatrix = aMat4;
}

void LightComponent::bindShadowProViewMatrix(unsigned int& aProgram)
{
	unsigned int loc = glGetUniformLocation(aProgram, "ShadowProjectionView");
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(mShadowProViewMatrix));
}

bool& LightComponent::shadowsAdded()
{
	return mShadowsAdded;
}

LightType LightComponent::getLightType()
{
	return mLightType;
}

const bool LightComponent::meshGenerated() const
{
	if (mLightTriCount > 0)
	{
		return true;
	}
	return false;
}

void LightComponent::clearMesh()
{
	mLightTriCount = 0;
}

void LightComponent::addTri(glm::vec4 aP0, glm::vec4 aP1, glm::vec4 aP2)
{
	//add all the vertices and indices to the correct
	//vectors
	//mVertices.push_back(aP0);
	//mVertices.push_back(aP1);
	//mVertices.push_back(aP2);
	mLightTri[mLightTriCount].P0 = aP0;
	mLightTri[mLightTriCount].P1 = aP1;
	mLightTri[mLightTriCount].P2 = aP2;

	mLightTriCount++;
}

void LightComponent::setLightType(LightType aType)
{
	mLightType = aType;
}

std::vector<glm::vec4>* LightComponent::getVertices()
{
	return &mVertices;
}

/*Private Function*/
void LightComponent::generateBuffers()
{
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mBUFFER_SIZE * sizeof(LightTri), mLightTri, GL_DYNAMIC_DRAW);
	//glGenBuffers(1, &mIBO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
