#pragma once

#ifndef __KEY_EVENT_H__
#define __KEY_EVENT_H__

#include "Event/Event.h"

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return mKeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(int aKeyCode)
		:mKeyCode(aKeyCode) {}
	int mKeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int aKeyCode, int aRepeatCount)
		:KeyEvent(aKeyCode), mRepeatCount(aRepeatCount) {}

	inline int GetRepeatCount() const { return mRepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int mRepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int aKeyCode)
		:KeyEvent(aKeyCode){}


	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << mKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

#endif // __KEY_EVENT_H__
