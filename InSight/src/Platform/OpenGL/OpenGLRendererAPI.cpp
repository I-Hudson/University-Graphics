#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace InSight
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4 & aColor)
	{
		glClearColor(aColor.r, aColor.g, aColor.b, aColor.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray)
	{
		glDrawElements(GL_TRIANGLES, aVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}