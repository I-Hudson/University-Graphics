#pragma once

#include "InSight/Renderer/RendererAPI.h"

namespace InSight
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& aColor) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray) override;
	};
}