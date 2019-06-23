#pragma once

#include "Renderer/RendererAPI.h"

namespace InSight
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& aColor)
		{
			sRendererAPI->SetClearColor(aColor);
		}

		inline static void Clear()
		{
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray)
		{
			sRendererAPI->DrawIndexed(aVertexArray);
		}


	private:
		static RendererAPI* sRendererAPI;
	};
}