#pragma once

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include "InSight/Event/Event.h"

struct GLFWwindow;

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& aTitle = "InSight Engine",
		unsigned int aWidth = 1920, unsigned int aHeight = 1080)
		:Title(aTitle), Width(aWidth), Height(aHeight)
	{}
};

//Interface representing a desktop system based window
class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& aCallback) = 0;
	virtual void SetVSync(bool aEnabled) = 0;
	virtual bool IsVSync() const = 0;
	virtual GLFWwindow* GetWindow() const = 0;

	static Window* Create(const WindowProps& aProps = WindowProps());
};

#endif // __WINDOW_H__
