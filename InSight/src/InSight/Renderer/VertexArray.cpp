#include "InSight/Renderer/VertexArray.h"
#include "InSight/Log.h"
#include "InSight/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace InSight
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			EN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;


		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}
		EN_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}