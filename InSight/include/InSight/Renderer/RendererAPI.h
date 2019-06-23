#pragma once

//-------------------------------------------------------------------------------
// Interface for each render API platform
//-------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <memory>
#include "Renderer/VertexArray.h"

namespace InSight
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Vulkan = 2
		};
		
		virtual void SetClearColor(const glm::vec4& aColor) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& aVertexArray) = 0;

		inline static API GetAPI() { return sAPI; }
	private:
		static API sAPI;
	};
}