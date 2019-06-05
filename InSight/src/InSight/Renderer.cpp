#include "Renderer.h"

Renderer::Renderer()
{
	//set screen size 
	mScreenSize = glm::vec2();
}

Renderer::~Renderer()
{
}

void Renderer::removeShader(Shader* a_removeShader)
{
	a_removeShader;
	for (unsigned int i = 0; i < mShadersToRender.size(); ++i)
	{
		mShadersToRender.erase(mShadersToRender.begin() + i);
	}
}

void Renderer::draw()
{
	std::vector<Shader*> renderedShaders;
	renderedShaders.reserve(mShadersToRender.size());
	std::vector<unsigned int> buffers;

	//for (unsigned int i = 0; i < mShadersToRender.size(); ++i)
	//{
	//	if (std::find(renderedShaders.begin(), renderedShaders.end(), mShadersToRender[i]) == renderedShaders.end())
	//	{
	//		renderedShaders.push_back(mShadersToRender[i]);

	//		//bind shader target
	//		mShadersToRender[i]->useShader(true);
	//	}
	//	else
	//	{
	//		//mMeshesToRender[i]->draw();
	//		mShadersToRender[i]->useShader();
	//	}
	//}

	//loop though all shaders
	for (auto& shader : mShadersToRender)
	{

		unsigned int fb;
		//if shader's gbuffer is valid. set fb to it 
		if (shader->getGBuffer() != nullptr && *shader->getGBuffer()->getFrameBuffer() != -1)
		{
			fb = *shader->getGBuffer()->getFrameBuffer();
		}
		//if shader's framebuffer is valid. set fb to it  
		else if(*shader->getFramebuffer() != -1)
		{
			fb = *shader->getFramebuffer();
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

			shader->useShader(true);
		}
		else
		{
			shader->useShader(false);
		}
	}
}

void Renderer::destroy()
{
	//destroy all shaders 
	for (auto& shader : mShadersToRender)
	{
		shader->destroy();
		delete shader;
	}
	mShadersToRender.clear();
}

void Renderer::setEntityManager(EntityManager* aEntityManager)
{
	//set entiy manager
	mEntityManager = aEntityManager;
}

EntityManager* Renderer::getEntityManager() const
{
	//get entity manager
	return mEntityManager;
}

void Renderer::setScreenSize(const glm::vec2& aScreenSize)
{
	//set screen size
	mScreenSize = aScreenSize;
}

glm::vec2 Renderer::getScreenSize()
{
	//get screen size
	return mScreenSize;
}
