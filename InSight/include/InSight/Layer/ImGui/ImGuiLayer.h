#pragma once

#ifndef __IMGUI_LAYER_H_
#define __IMGUI_LAYER_H_

#include "Layer/Layer.h"

namespace InSight
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& aEvent) override;

	private:
		float mTime = 0.0f;
	};
}
#endif // __IMGUI_LAYER_H_
