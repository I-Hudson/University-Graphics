#include "Platform/Windows/WindowsWindow.h"
#include "Log.h"
#include <iostream>

static bool sGLFWInitialized = false;

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

		sGLFWInitialized = true;
	}

	mWindow = glfwCreateWindow((int)aProps.Width, (int)aProps.Height, mData.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, &mData);
	SetVSync(true);
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


