#include "Platform/Windows/WindowsWindow.h"
#include "Log.h"
#include <iostream>

#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

static bool sGLFWInitialized = false;
static void GLFWErrorCallback(int aError, const char* aDesc)
{
	EN_CORE_ERROR("GLFW Error ({0}): {1}", aError, aDesc);
}

Window* Window::Create(const WindowProps& aProps)
{
	return new WindowsWindow(aProps);
}

WindowsWindow::WindowsWindow(const WindowProps& aProps)
{
	Init(aProps);
}

WindowsWindow::~WindowsWindow()
{
	Shutdown();
}

void WindowsWindow::Init(const WindowProps& aProps)
{
	mData.Title = aProps.Title;
	mData.Width = aProps.Width;
	mData.Height = aProps.Height;

	//EN_TRACE("Creating window {0} ({1}, {2})", aProps.Title, aProps.Width, aProps.Height);

	if (!sGLFWInitialized)
	{
		int success = glfwInit();
		
		std::cout << success << std::endl;
		glfwSetErrorCallback(GLFWErrorCallback);
		sGLFWInitialized = true;
	}

	mWindow = glfwCreateWindow((int)aProps.Width, (int)aProps.Height, mData.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, &mData);
	SetVSync(true);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}

			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}

			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
		}
	});

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

	glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double aXOffset, double aYOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)aXOffset, (float)aYOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double aXPos, double aYPos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float)aXPos, (float)aYPos);
		data.EventCallback(event);
	});
}

void WindowsWindow::Shutdown()
{
	glfwDestroyWindow(mWindow);
}

void WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(mWindow);
}

void WindowsWindow::SetVSync(bool aEnabled)
{
	if (aEnabled)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
	mData.VSync = aEnabled;
}

bool WindowsWindow::IsVSync() const
{
	return mData.VSync;
}


