#pragma once

#ifndef __GUI_H__
#define __GUI_H__

#include <vector>
#include <sstream>

#include "Log.h"
#include "GUIBase.h"

class GUI
{
public:
	GUI();
	~GUI();

	void update();
	void draw();

	//Add new elements to the GUI
	template<typename T, typename... TArgs>
	void addGUIElement(TArgs&&... aArgs)
	{
		if (!hasElement<T>())
		{
			T* element(DEBUG_NEW  T(std::forward<TArgs>(aArgs)...));
			mGUIElements.emplace_back(element);
			mGUIElements[mGUIElements.size() - 1]->init(this);
		}
		else
		{
			std::stringstream ss;
			ss << "GUI Element :";
			ss << typeid(T).name();
			ss << ": already exists";
			EN_WARN(ss.str().c_str());
		}
	}

	template<typename T>
	T* getGUIElement()
	{
		for (auto& element : mGUIElements)
		{
			if (dynamic_cast<T*>(element))
			{
				return dynamic_cast<T*>(element);
			}
		}
		return nullptr;
	}

	template<typename T>
	bool hasElement()
	{
		for (auto& element : mGUIElements)
		{
			if (dynamic_cast<T*>(element))
			{
				return true;
			}
		}
		return false;
	}

	//Remove a single element from the GUI
	template<typename T>
	void removeGUIElement()
	{
		for (int i = 0; i < mGUIElements.size(); ++i)
		{
			if (element == T)
			{
				delete element;
				mGUIElements.erase(mGUIElements.begin() + i);
			}
		}
	}

private:
	std::vector<GUIBase*> mGUIElements;
};

#endif // !__GUI_H__
