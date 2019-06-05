#pragma once

#ifndef __APPLICATION_EVENT_H__
#define __APPLICATION_EVENT_H__

#include "Event/Event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int aWidth, unsigned int aHeight)
		: mWidth(aWidth), mHeight(aHeight) {}

	inline unsigned int GetWidth() const { return mWidth; }
	inline unsigned int GetHeight() const { return mHeight; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	unsigned int mWidth, mHeight;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() {}

	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

#endif // __APPLICATION_EVENT_H__
