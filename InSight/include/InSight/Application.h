#pragma once

#include <glad/glad.h>
#include "Debug.h"
#include "Platform/Window.h"
#include "Layer/LayerStack.h"
#include "InSight/InSight.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "InSight/Layer/ImGui/ImGuiLayer.h"

#include "InSight/Renderer/Shader.h"
#include "InSight/Renderer/Buffer.h"

struct GLFWwindow;

namespace InSight
{
	class Application
	{
	public:

		Application();
		virtual ~Application() {}

		bool	create(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

		void	run(const char* a_name, int a_width, int a_height, bool a_bFullscreen);

		void	OnEvent(Event& aE);

		void PushLayer(Layer* aLayer);
		void PushOverlay(Layer* aLayer);

		inline Window& GetWindow() { return *mWindow; }
		inline static Application& Get() { return *sInstnace; }

		void	quit() { m_running = false; }

		void	showFrameData(bool a_showFrameData);

	protected:
		bool OnWindowClose(WindowCloseEvent& aEvent);
		std::unique_ptr<Window> mWindow;
		InSight::ImGuiLayer*  mImGuiLayer;
		LayerStack mLayerStack;

		unsigned int mVertexArray;
		std::unique_ptr<InSight::VertexBuffer> mVertexBuffer;
		std::unique_ptr<InSight::IndexBuffer> mIndexB;
		std::unique_ptr<InSight::Shader> mShader;


		virtual bool	onCreate() = 0;
		virtual void	Update(float a_deltaTime) = 0;
		virtual void	Draw() = 0;
		virtual void	Destroy() = 0;

		bool			m_running;
		bool			m_showFrameData;

	private:
		static Application* sInstnace;
	};
}