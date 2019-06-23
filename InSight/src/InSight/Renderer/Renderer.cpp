#include "InSight/Renderer/Renderer.h"

namespace InSight
{
	void Renderer::BegineScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& aVertexArrray)
	{
		aVertexArrray->Bind();
		RenderCommand::DrawIndexed(aVertexArrray);
	}
}