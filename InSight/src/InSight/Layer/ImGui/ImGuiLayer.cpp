#include "Layer/ImGui/ImGuiLayer.h"

#include "Application.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include <imgui_impl_glfw.h>

namespace InSight
{
	#define EN_BIND_EVENT_FN(fn) std::bind(&ImGuiLayer::fn, this, std::placeholders::_1)

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		//Set Up IMGUI
		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		// Setup Dear ImGui style

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		// TEMPORAY
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		const char* glsl_version = "#version 400";

		// Setup Platform/Renderer bindings
		//ImGui_ImplGlfw_InitForOpenGL(Application::Get().GetWindow().GetWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = mTime > 0.0f ? (time - mTime) : (1.0f / 60.0f);
		mTime = time;

		ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event & aEvent)
	{
		EventDispatcher dispatcher(aEvent);
		dispatcher.Dispatch<MouseButtonPressedEvent>(EN_BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EN_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(EN_BIND_EVENT_FN(OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(EN_BIND_EVENT_FN(OnMouseScolledEvent));

		dispatcher.Dispatch<KeyPressedEvent>(EN_BIND_EVENT_FN(OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(EN_BIND_EVENT_FN(OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(EN_BIND_EVENT_FN(OnKeyTypedEvent));

		dispatcher.Dispatch<WindowResizeEvent>(EN_BIND_EVENT_FN(OnWindowResizeEvent));
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
		return false;

	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent & aEvent)
	{
		return false;

	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent & aEvent)
	{
		return false;

	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent & aEvent)
	{
		return false;
	}
}
