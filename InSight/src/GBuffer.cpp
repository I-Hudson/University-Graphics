#include "..\include\GBuffer.h"

GBuffer::GBuffer()
{
	//set mHasBeenDeleted to false
	mHasBeenDeleted = false;
}

GBuffer::~GBuffer()
{
}

void GBuffer::destroy()
{
	//destroy / delete
	if (mFrameBuffer != -1)
	{
		glDeleteFramebuffers(1, &mFrameBuffer);
	}

	for (auto& texture : mFrameBufferTextures)
	{
		glDeleteTextures(1, &texture.mTexture);
	}
	mFrameBufferTextures.clear();

	mHasBeenDeleted = true;
}

void GBuffer::createFrameBuffer()
{
	//gen framebuffer
	glGenFramebuffers(1, &mFrameBuffer);
}

unsigned int & GBuffer::attachTextureToFrameBuffer(int aLevel, int aInternalFormat, 
	int aWidth, int aHeight, 
	int aBoarder, GLenum aFormat,
	GLenum aType, const void * aPixels,
	GLenum aAttachment, const char* aName)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	unsigned int fbo;

	//gen texture, set texture props
	glGenTextures(1, &fbo);
	glBindTexture(GL_TEXTURE_2D, fbo);
	glTexImage2D(GL_TEXTURE_2D, aLevel, aInternalFormat, aWidth, aHeight, aBoarder, aFormat, aType, aPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (aAttachment == GL_DEPTH_ATTACHMENT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glFramebufferTexture(GL_FRAMEBUFFER, aAttachment, fbo, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	FBTexture fbTexture;
	fbTexture.mName = aName;
	fbTexture.mTextureSize = glm::vec2(aWidth, aHeight);
	fbTexture.mTexture = fbo;

	//add texture to mFrameBufferTextures
	mFrameBufferTextures.push_back(fbTexture);

	//update draw buffers
	if (aAttachment != GL_DEPTH_STENCIL_ATTACHMENT &&
		aAttachment != GL_COLOR_ATTACHMENT4 && aAttachment != GL_COLOR_ATTACHMENT5 && aAttachment != GL_DEPTH_ATTACHMENT)
	{
		mDrawBuffers.push_back(aAttachment);
		glDrawBuffers(mDrawBuffers.size(), mDrawBuffers.data());
	}

	//check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{

		printf("Framebuffer Error\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

unsigned int* GBuffer::getFrameBuffer()
{
	//get framebuffer
	return &mFrameBuffer;
}


unsigned int* GBuffer::getTexture(int aIndex)
{
	//get a texture
	if (mFrameBufferTextures.size() > 0)
	{
		return &mFrameBufferTextures[aIndex].mTexture;
	}
	else
	{
		return 0;
	}
}

std::vector<FBTexture>* GBuffer::getAllTextures()
{
	//get all textures
	return &mFrameBufferTextures;
}

void GBuffer::setDrawBuffers()
{
	//set draw buffers
	glDrawBuffers(mDrawBuffers.size(), mDrawBuffers.data());
}

void GBuffer::bindBuffer()
{
	//bind this buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
}

void GBuffer::unBindBuffer()
{
	//unbind this buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool GBuffer::hasBeenDeleted()
{
	//check if this has been deleted
	if (this != nullptr)
	{
		if (mHasBeenDeleted)
		{
			return true;
		}
		return false;
	}
	return true;
}

void GBuffer::setScreenSize(const glm::vec2& aScreenSize)
{
	//set screen size for this buffer
	mScreenSize = aScreenSize;
}

glm::vec2* GBuffer::getScreenSize()
{
	//get screen size for this buffer
	return &mScreenSize;
}

void GBuffer::setViewPort()
{
	//set view port size
	glViewport(0, 0, mScreenSize.x, mScreenSize.y);
}
