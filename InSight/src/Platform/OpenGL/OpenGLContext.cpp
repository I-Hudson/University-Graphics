#include "Platform/OpenGL/OpenGLContext.h"
#include "InSight/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace InSight
{
	OpenGLContext::OpenGLContext(GLFWwindow * aWindowHandle)
		: mWindowHandle(aWindowHandle)
	{
		EN_CORE_ASSERT(mWindowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindowHandle);
		int gladStatus = gladLoadGL();
		EN_CORE_ASSERT(gladStatus, "Failed to initailize GLAD!");

		//int major = glfwGetWindowAttrib(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()), GLFW_CONTEXT_VERSION_MAJOR);
		//int minor = glfwGetWindowAttrib(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()), GLFW_CONTEXT_VERSION_MINOR);
		//int revision = glfwGetWindowAttrib(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()), GLFW_CONTEXT_REVISION);
		
		////log->addLog
		
		//EN_CORE_INFO("OpenGL Version {0}.{1}.{2}", major, minor, revision);
		EN_CORE_INFO("OpenGL Info:");
		EN_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		EN_CORE_INFO("  BaseRenderer: {0}", glGetString(GL_BaseRenderer));
		EN_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}
}