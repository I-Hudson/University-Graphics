#include "Sandbox.h"

class EampleLayer : public Layer
{
public:
	EampleLayer()
		:Layer("Example")
	{
	}

	void OnUpdate() override
	{
		EN_INFO("ExampleLayer::Update");
	}

	void OnEvent(Event& aEvent) override
	{
		EN_TRACE("{0}", aEvent);
	}
};

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

bool Sandbox::onCreate()
{
	PushLayer(new EampleLayer());
	//PushOverlay(new InSight::ImGuiLayer());

	return true;
}