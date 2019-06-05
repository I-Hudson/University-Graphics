#pragma once

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <string>
#include "InSight/Event/Event.h"

struct GLFWwindow;

//window props
struct WindowProps
{
	//Title, width and height
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	//Constructor
	WindowProps(const std::string& aTitle = "InSight Engine",
		unsigned int aWidth = 1920, unsigned int aHeight = 1080)
		:Title(aTitle), Width(aWidth), Height(aHeight)
	{}
};

//Interface representing a desktop system based window
class Window
{
public:
	//event callback
	using EventCallbackFn = std::function<void(Event&)>;

	//destructor
	virtual ~Window() {}

	//update
	virtual void OnUpdate() = 0;

	//width
	virtual unsigned int GetWidth() const = 0;
	//height
	virtual unsigned int GetHeight() const = 0;

	//set the event callback
	virtual void SetEventCallback(const EventCallbackFn& aCallback) = 0;
	//set vsync
	virtual void SetVSync(bool aEnabled) = 0;
	//is vsync on or off
	virtual bool IsVSync() const = 0;
	//get the glfw window pointer
	virtual GLFWwindow* GetWindow() const = 0;

	//create window
	static Window* Create(const WindowProps& aProps = WindowProps());
};

#endif // __WINDOW_H__
