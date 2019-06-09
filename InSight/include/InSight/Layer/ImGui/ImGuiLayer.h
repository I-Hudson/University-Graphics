#pragma once

#ifndef __IMGUI_LAYER_H_
#define __IMGUI_LAYER_H_

#include "Layer/Layer.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace InSight
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& aEvent) override;

		void Begin();
		void End();

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& aEvent);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& aEvent);
		bool OnMouseMoveEvent(MouseMovedEvent& aEvent);
		bool OnMouseScolledEvent(MouseScrolledEvent& aEvent);

		bool OnKeyPressedEvent(KeyPressedEvent& aEvent);
		bool OnKeyReleasedEvent(KeyReleasedEvent& aEvent);
		bool OnKeyTypedEvent(KeyTypedEvent& aEvent);

		bool OnWindowResizeEvent(WindowResizeEvent& aEvent);

	private:
		float mTime = 0.0f;
	};
}
#endif // __IMGUI_LAYER_H_
