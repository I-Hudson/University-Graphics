#include "InSight/Renderer/Buffer.h"
#include "InSight/Log.h"
#include "InSight/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace InSight
{
	VertexBuffer* VertexBuffer::Create(float* aVertices, uint32_t aSize)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;


			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(aVertices, aSize);
		}
		EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* aIndices, uint32_t aSize)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;


		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(aIndices, aSize);
		}
		EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}