#include "Application.h"
#include "Utilities.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
//Include GLAD GL Extension defines
#include <glad/glad.h>
//Include GLFW header
#include <GLFW/glfw3.h>
//Dear ImGUI includes
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//include the application log to record details
#include "Application_Log.h"
#include "Log.h"
#include "Gizmos.h"

#define BUILD_VERSION_MAJOR 1
#define BUILD_VERSION_MINOR 1
#define BULID_VERSION_REVISION 1

#ifdef __GL_DEBUG__
void APIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
{
	char *strSource = "0";
	char *strType = strSource;
	char *strSeverity = strSource;
	switch (source) {
	case GL_DEBUG_SOURCE_API:     strSource = "API";						break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:       strSource = "WINDOWS";		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:     strSource = "SHADER COMP.";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:         strSource = "3RD PARTY";		break;
	case GL_DEBUG_SOURCE_APPLICATION:         strSource = "APP";			break;
	case GL_DEBUG_SOURCE_OTHER:               strSource = "OTHER";			break;
	}
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:		        strType = "ERROR";				break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:   strType = "Deprecated";		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   strType = "Undefined";			break;
	case GL_DEBUG_TYPE_PORTABILITY:           strType = "Portability";		break;
	case GL_DEBUG_TYPE_PERFORMANCE:           strType = "Performance";		break;
	case GL_DEBUG_TYPE_OTHER:                 strType = "Other";			break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:	        strSeverity = "High";			break;
	case GL_DEBUG_SEVERITY_MEDIUM:          strSeverity = "Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:             strSeverity = "Low";			break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	strSeverity = "Notification";	break;
	}
	static int errNum = 0;

	Application_Log* log = Application_Log::Get();
	if (log != nullptr)
	{
		log->addLog(LOG_LEVEL::LOG_ERROR, "%i: (GL Error Callback) Error Source: %s, Error Type: %s, Severity: %s, Message: %s", strSource, strType, strSeverity, msg);
	}

	std::fstream file;
	file.open("GL_Error.log", std::ios_base::out | std::ios_base::app);

	file << errNum << ": GL Error Callback: " << "\n Error Source: " << strSource << "\n Error Type: " << strType << "\n Severity: " << strSeverity << "\n Message: " << msg << std::endl;
	file.close();
	++errNum;
}
#endif

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::sInstnace = nullptr;

Application::Application()
	: m_running(false)
{
	EN_CORE_ASSERT(!sInstnace, "Application already exists!");
	sInstnace = this;
}

bool Application::create(const char* a_name, int a_width, int a_height, bool a_bFullscreen )
{
	//// initialise glfw systems
	//if (glfwInit() != GL_TRUE)
	//{
	//	std::cout << "Unable to initialize GLFW" << std::endl;
	//	return false;
	//}
	//m_windowWidth = a_width;
	//m_windowHeight = a_height;
	//// create a windowed mode window and its OpenGL context
	//m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_name, ( a_bFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	//if (m_window == nullptr)
	//{
	//	std::cout << "Unable to create a GLFW Window" << std::endl;
	//	glfwTerminate();
	//	return false;
	//}
	//glfwMakeContextCurrent(m_window);

	mWindow = std::unique_ptr<Window>(Window::Create(WindowProps(a_name, a_width, a_height)));
	mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

	//if (!glLoadGLL()) {
	//	glfwDestroyWindow(mWindow->GetWindow());
	//	glfwTerminate();
	//	return false;
	//}


	
	//glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h)
	//{
	//	glViewport(0, 0, w, h);
	//});
	
#ifdef __GL_DEBUG__
	glDebugMessageCallback(glErrorCallback, NULL);
#endif

	//Set Up IMGUI
	//Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//const char* glsl_version = "#version 400";

	// Setup Platform/Renderer bindings
	//ImGui_ImplGlfw_InitForOpenGL(mWindow->GetWindow() , true);
	//ImGui_ImplOpenGL3_Init(glsl_version);

	//start the logger
	Engine::Log::Init();

	Application_Log* log = Application_Log::Create();
	if (log != nullptr)
	{

		//log->addLog
		EN_CORE_INFO("UG Framework Version: %i.%i.%i", BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR, BULID_VERSION_REVISION);
		int major = glfwGetWindowAttrib(mWindow->GetWindow(), GLFW_CONTEXT_VERSION_MAJOR);
		int minor = glfwGetWindowAttrib(mWindow->GetWindow(), GLFW_CONTEXT_VERSION_MINOR);
		int revision = glfwGetWindowAttrib(mWindow->GetWindow(), GLFW_CONTEXT_REVISION);

		//log->addLog
		EN_CORE_INFO("OpenGL Version %i.%i.%i", major, minor, revision);
	}
	m_showFrameData = true;



	bool result = onCreate();
	if (result == false)
	{
		glfwTerminate();
	}
	return result;
}

void Application::run(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
{
	if (create(a_name, a_width, a_height, a_bFullscreen))
	{
		Utility::resetTimer();
		m_running = true;
		do
		{
			float deltaTime = Utility::tickTimer();

			// Start the Dear ImGui frame
			//ImGui_ImplOpenGL3_NewFrame();
			//ImGui_ImplGlfw_NewFrame();
			//ImGui::NewFrame();

			//ImGui Set up Framerate window
			//showFrameData(true);

			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate();
			}
			Update(deltaTime);

			Draw();

			//ImGui::Render();

			//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			mWindow->OnUpdate();
			//glfwSwapBuffers(m_window);
			//glfwPollEvents();

		} while (m_running == true && glfwWindowShouldClose(mWindow->GetWindow()) == 0);

		Destroy();
	}

	Application_Log::Destroy();
	Gizmos::destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Application::OnWindowClose(WindowCloseEvent& aEvent)
{
	m_running = false;
	return true;
}

void Application::OnEvent(Event& aE)
{
	EventDispatcher dispatcher(aE);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	for (auto it = mLayerStack.end(); it != mLayerStack.begin(); )
	{
		(*--it)->OnEvent(aE);
		if (aE.Handled)
		{
			break;
		}
	}
}

void Application::PushLayer(Layer * aLayer)
{
	mLayerStack.PushLayer(aLayer);
	aLayer->OnAttach();
}

void Application::PushOverlay(Layer * aLayer)
{
	mLayerStack.PushOverlay(aLayer);
	aLayer->OnAttach();
}

void Application::showFrameData(bool a_showFrameData)
{
	m_showFrameData = a_showFrameData;
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
	if (ImGui::Begin("Frame Data", &m_showFrameData, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Separator();
		ImGui::Text("Application Average: %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (&m_showFrameData && ImGui::MenuItem("Close")) m_showFrameData = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
	
}


