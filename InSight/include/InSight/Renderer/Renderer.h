#pragma once

namespace InSight
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
		Vulkan = 2
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return sRendererAPI; }

	private:
		static RendererAPI sRendererAPI;
	};

}