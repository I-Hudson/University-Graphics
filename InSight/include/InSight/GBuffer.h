#pragma once

#ifndef __GBUFFER_H__
#define __GBUFFER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

//texture struct
struct FBTexture
{
	std::string mName;
	glm::vec2 mTextureSize;
	unsigned int mTexture;
};

class GBuffer
{
public:
	//constructor
	GBuffer();
	//destructor
	~GBuffer();

	//destroy this gbuffer
	void destroy();

	//create a framebuffer object
	void createFrameBuffer();
	//attach a new render target to this gbuffer
	unsigned int& attachTextureToFrameBuffer(int aLevel, int aInternalFormat,
		int aWidth, int aHeight,
		int aBoarder, GLenum aFormat,
		GLenum aType, const void* aPixels,
		GLenum aAttachment, const char* aName);

	//get the framebuffer object
	unsigned int* getFrameBuffer();

	//get a texture
	unsigned int* getTexture(int aIndex);
	//return a vector of all the render targets this gbuffer renders too
	std::vector<FBTexture>* getAllTextures();

	//set the buffers to draw top
	void setDrawBuffers();

	//bind buffer
	void bindBuffer();
	//unbind buffer
	void unBindBuffer();

	//check if this buffer has been deleted
	bool hasBeenDeleted();

	//set the screen size for this gbuffer
	void setScreenSize(const glm::vec2& aScreenSize);
	//get this screen size for this gbuffer
	glm::vec2* getScreenSize();
	//set the view port for this gbuffer
	void setViewPort();

private:
	unsigned int mFrameBuffer = -1;
	std::vector<FBTexture> mFrameBufferTextures;
	std::vector<GLenum> mDrawBuffers;

	//set buffer screensize
	glm::vec2 mScreenSize;

	//gbuffer deleted
	bool mHasBeenDeleted;
};

#endif // !__G_BUFFER_H__
