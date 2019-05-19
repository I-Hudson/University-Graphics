#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::addShader(Shader* a_addShader)
{
	if (std::find(mShadersToRender.begin(), mShadersToRender.end(), a_addShader) == mShadersToRender.end())
	{
		mShadersToRender.push_back(a_addShader);
	}
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

	for (auto& shader : mShadersToRender)
	{
		unsigned int fb;
		if (shader->getGBuffer() != nullptr && *shader->getGBuffer()->getFrameBuffer() != -1)
		{
			fb = *shader->getGBuffer()->getFrameBuffer();
		}
		else if(*shader->getFramebuffer() != -1)
		{
			fb = *shader->getFramebuffer();
		}
		else
		{
			fb = 0;
		}
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
