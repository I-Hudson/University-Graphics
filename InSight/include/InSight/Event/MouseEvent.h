#pragma once

#ifndef __MOUSE_EVENT_H__
#define __MOUSE_EVENT_H__

#include "Event.h"


class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float aX, float aY)
		: mMouseX(aX), mMouseY(aY) {}

	inline float GetX() const { return mMouseX; }
	inline float GetY() const { return mMouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float mMouseX, mMouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float aXOffset, float aYOffset)
		: mXOffset(aXOffset), mYOffset(aYOffset) {}

	inline float GetXOffset() const { return mXOffset; }
	inline float GetYOffset() const { return mYOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float mXOffset, mYOffset;
};

class MouseButtonEvent : public Event
{
public:
	inline int GetMouseButton() const { return mButton; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	MouseButtonEvent(int aButton)
		: mButton(aButton) {}

	int mButton;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int aButton)
		: MouseButtonEvent(aButton) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << mButton;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int aButton)
		: MouseButtonEvent(aButton) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << mButton;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};
#endif // __MOUSE_EVENT_H__