#include "Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace InSight
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}