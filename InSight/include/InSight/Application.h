#pragma once

#include <glad/glad.h>
#include <glm/common.hpp>
#include "Debug.h"
#include "Platform/Window.h"
#include "Layer/LayerStack.h"
#include "InSight/InSight.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "InSight/Layer/ImGui/ImGuiLayer.h"

#include "InSight/Renderer/Shader.h"
#include "InSight/Renderer/Buffer.h"

#include "InSight/Scene/SceneManager.h"

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

		struct Vertex
		{
			glm::vec4 Position;
			glm::vec4 Colour;
			glm::vec4 Normal;
			glm::vec2 TexCoord1;

			Vertex(const glm::vec4& aPosition, const glm::vec4& aColour, const glm::vec4& aNormal, const glm::vec2& aTexCoord1)
				:Position(aPosition), Colour(aColour), Normal(aNormal), TexCoord1(aTexCoord1)
			{}

			Vertex(const float& aPositionX, const float& aPositionY, const float& aPositionZ, const float& aPositionW,
				const float& aColourR, const float& aColourG, const float& aColourB, const float& aColourA,
				const float& aNormalX, const float& aNormalY, const float& aNormalZ, const float& aNormalW,
				const float& aTexCoord1X, const float& aTexCoord1Y)
				: Position(glm::vec4(aPositionX, aPositionY, aPositionZ, aPositionW)),
				Colour(glm::vec4(aColourR, aColourG, aColourB, aColourA)),
				Normal(glm::vec4(aNormalX, aNormalY, aNormalZ, aNormalW)),
				TexCoord1(glm::vec2(aTexCoord1X, aTexCoord1Y))
			{}
		};

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