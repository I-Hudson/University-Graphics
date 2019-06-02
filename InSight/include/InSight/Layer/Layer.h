#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include <string>
#include "Event/Event.h"

class Layer
{
public:
	Layer(const std::string& aName = "Layer");
	virtual ~Layer();

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate();
	virtual void OnEvent(Event& aEvent);

	inline const std::string& GetName() const {return mDebugName; }

protected:
	std::string mDebugName;
};

#endif // __LAYER_H__
