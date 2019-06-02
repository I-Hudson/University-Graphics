#pragma once

#ifndef __WINDOWS_WINDOW_H__
#define __WINDOWS_WINDOW_H__

#include "Platform/Window.h"
#include <GLFW/glfw3.h>

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProps& aProps);
	virtual ~WindowsWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override { return mData.Width; };
	inline unsigned int GetHeight() const override { return mData.Height; };

	inline void SetEventCallback(const EventCallbackFn& aCallback) override { mData.EventCallback = aCallback; };
	void SetVSync(bool aEnabled) override;
	bool IsVSync() const override;
	GLFWwindow* GetWindow() const { return mWindow; }

private:
	virtual void Init(const WindowProps& aProps);
	virtual void Shutdown();

	GLFWwindow* mWindow;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData mData;
};

#endif // __WINDOWS_WINDOW_H__
