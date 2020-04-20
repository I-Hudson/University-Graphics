#include "Layer/ImGui/ImGuiLayer.h"

#include "Application.h"

#include "Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace InSight
{
	#define EN_BIND_EVENT_FN(fn) std::bind(&ImGuiLayer::fn, this, std::placeholders::_1)

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		//Set Up IMGUI
		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		//set style
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		const char* glsl_version = "#version 400";
		// Setup Platform/BaseRenderer bindings

		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		else if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
		{

			ImGui_ImplGlfw_InitForVulkan(window, true);
		}
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
	}

	void ImGuiLayer::OnEvent(Event & aEvent)
	{
		//EventDispatcher dispatcher(aEvent);
		//dispatcher.Dispatch<MouseButtonPressedEvent>(EN_BIND_EVENT_FN(OnMouseButtonPressedEvent));
		//dispatcher.Dispatch<MouseButtonReleasedEvent>(EN_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
		//dispatcher.Dispatch<MouseMovedEvent>(EN_BIND_EVENT_FN(OnMouseMoveEvent));
		//dispatcher.Dispatch<MouseScrolledEvent>(EN_BIND_EVENT_FN(OnMouseScolledEvent));
		//
		//dispatcher.Dispatch<KeyPressedEvent>(EN_BIND_EVENT_FN(OnKeyPressedEvent));
		//dispatcher.Dispatch<KeyReleasedEvent>(EN_BIND_EVENT_FN(OnKeyReleasedEvent));
		//dispatcher.Dispatch<KeyTypedEvent>(EN_BIND_EVENT_FN(OnKeyTypedEvent));
		//
		//dispatcher.Dispatch<WindowResizeEvent>(EN_BIND_EVENT_FN(OnWindowResizeEvent));
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[aEvent.GetMouseButton()] = true;

		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[aEvent.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnMouseMoveEvent(MouseMovedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(aEvent.GetX(), aEvent.GetY());

		return false;
	}
	bool ImGuiLayer::OnMouseScolledEvent(MouseScrolledEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += aEvent.GetXOffset();
		io.MouseWheel += aEvent.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[aEvent.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[aEvent.GetKeyCode()] = false;

		return false;

	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = aEvent.GetKeyCode();

		if (keycode > 0 && keycode < 0x10000)
		{
			//io.AddInputCharacter((unsigned short)keycode);
		}

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent & aEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(aEvent.GetWidth(), aEvent.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, aEvent.GetWidth(), aEvent.GetHeight());

		return false;
	}
}
