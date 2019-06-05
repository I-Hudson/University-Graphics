#pragma once

#ifndef __WINDOWS_WINDOW_H__
#define __WINDOWS_WINDOW_H__

#include "Platform/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowsWindow : public Window
{
public:
	//construcotr
	WindowsWindow(const WindowProps& aProps);
	//destructor
	virtual ~WindowsWindow();

	//update
	void OnUpdate() override;

	//width
	inline unsigned int GetWidth() const override { return mData.Width; };
	//height
	inline unsigned int GetHeight() const override { return mData.Height; };

	//set event callback
	inline void SetEventCallback(const EventCallbackFn& aCallback) override { mData.EventCallback = aCallback; };
	//set vsync
	void SetVSync(bool aEnabled) override;
	//is vsync on or off
	bool IsVSync() const override;
	//get the glfw window pointer
	GLFWwindow* GetWindow() const { return mWindow; }

private:
	//init func
	virtual void Init(const WindowProps& aProps);
	//shutdown
	virtual void Shutdown();

	//glfww window pointer
	GLFWwindow* mWindow;

	//window data
	struct WindowData
	{
		//title
		std::string Title;
		//width, height
		unsigned int Width, Height;
		//vsync
		bool VSync;

		//event callback
		EventCallbackFn EventCallback;
	};

	//Window data
	WindowData mData;
};

#endif // __WINDOWS_WINDOW_H__
