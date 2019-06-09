#pragma once

#include "InSight/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace InSight
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* aWindowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}