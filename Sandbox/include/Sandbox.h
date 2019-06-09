#pragma once

#include "InSight/Application.h"
#include <glm/glm.hpp>

#include "InSight/Log.h"
#include "Layer/LayerStack.h";
#include "Layer/ImGui/ImGuiLayer.h"

// Derived application class that wraps up all globals neatly
class Sandbox : public InSight::Application
{
public:
	Sandbox();
	virtual ~Sandbox();

protected:

	virtual bool onCreate();
	virtual void Update(float a_deltaTime) {}
	virtual void Draw() {}
	virtual void Destroy() {}
};