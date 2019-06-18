#include "BaseRenderer.h"

BaseRenderer::BaseRenderer()
{
	//set screen size 
	mScreenSize = glm::vec2();
}

BaseRenderer::~BaseRenderer()
{
}

void BaseRenderer::removeShader(BaseShader* a_removeShader)
{
	a_removeShader;
	for (unsigned int i = 0; i < mShadersToRender.size(); ++i)
	{
		mShadersToRender.erase(mShadersToRender.begin() + i);
	}
}

void BaseRenderer::draw()
{
	std::vector<BaseShader*> renderedShaders;
	renderedShaders.reserve(mShadersToRender.size());
	std::vector<unsigned int> buffers;

	//for (unsigned int i = 0; i < mShadersToRender.size(); ++i)
	//{
	//	if (std::find(renderedShaders.begin(), renderedShaders.end(), mShadersToRender[i]) == renderedShaders.end())
	//	{
	//		renderedShaders.push_back(mShadersToRender[i]);

	//		//bind BaseShader target
	//		mShadersToRender[i]->useShader(true);
	//	}
	//	else
	//	{
	//		//mMeshesToRender[i]->draw();
	//		mShadersToRender[i]->useShader();
	//	}
	//}

	//loop though all shaders
	for (auto& BaseShader : mShadersToRender)
	{

		unsigned int fb;
		//if BaseShader's gbuffer is valid. set fb to it 
		if (BaseShader->getGBuffer() != nullptr && *BaseShader->getGBuffer()->getFrameBuffer() != -1)
		{
			fb = *BaseShader->getGBuffer()->getFrameBuffer();
		}
		//if BaseShader's framebuffer is valid. set fb to it  
		else if(*BaseShader->getFramebuffer() != -1)
		{
			fb = *BaseShader->getFramebuffer();
		}
		else
		{
			//fb is backbuffer
			fb = 0;
		}
		//has buffer been rendered to before. if not clear it.
		if (std::find(buffers.begin(), buffers.end(), fb) == buffers.end())
		{
			buffers.push_back(fb);

			BaseShader->useShader(true);
		}
		else
		{
			BaseShader->useShader(false);
		}
	}
}

void BaseRenderer::destroy()
{
	//destroy all shaders 
	for (auto& BaseShader : mShadersToRender)
	{
		BaseShader->destroy();
		delete BaseShader;
	}
	mShadersToRender.clear();
}

void BaseRenderer::setScreenSize(const glm::vec2& aScreenSize)
{
	//set screen size
	mScreenSize = aScreenSize;
}

glm::vec2 BaseRenderer::getScreenSize()
{
	//get screen size
	return mScreenSize;
}
