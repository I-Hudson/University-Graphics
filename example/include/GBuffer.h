#pragma once

#ifndef __G_BUFFER_H__
#define __G_BUFFER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct FBTexture
{
	const char* mName;
	glm::vec2 mTextureSize;
	unsigned int mTexture;
};

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	void destroy();

	void createFrameBuffer();
	unsigned int& attachTextureToFrameBuffer(int aLevel, int aInternalFormat,
		int aWidth, int aHeight,
		int aBoarder, GLenum aFormat,
		GLenum aType, const void* aPixels,
		GLenum aAttachment, const char* aName);

	unsigned int* getFrameBuffer();

	unsigned int* getTexture(int aIndex);
	std::vector<FBTexture>* getAllTextures();

	void setDrawBuffers();

	void bindBuffer();
	void unBindBuffer();

	bool hasBeenDeleted();

private:
	unsigned int mFrameBuffer = -1;
	std::vector<FBTexture> mFrameBufferTextures;
	std::vector<GLenum> mDrawBuffers;

	bool mHasBeenDeleted;
};

#endif // !__G_BUFFER_H__
