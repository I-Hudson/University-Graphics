#pragma once

#include "Renderer/RenderCommand.h"

namespace InSight
{

	class Renderer
	{
	public:
		static void BegineScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& aVertexArrray);


		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}