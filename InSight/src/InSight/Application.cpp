#include "Application.h"
#include "Utilities.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
//Include GLAD GL Extension defines
#include <glad/glad.h>

//Dear ImGUI includes
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//include the application log to record details
#include "Application_Log.h"
#include "Log.h"
#include "Gizmos.h"
#include "Platform/Windows/WindowsInput.h"

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
	case GL_DEBUG_SOURCE_SHADER_COMPILER:     strSource = "BaseShader COMP.";	break;
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

namespace InSight
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::sInstnace = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType aType)
	{
		switch (aType)
		{
			case InSight::ShaderDataType::Float:	return GL_FLOAT;
			case InSight::ShaderDataType::Float2:	return GL_FLOAT;
			case InSight::ShaderDataType::Float3:	return GL_FLOAT;
			case InSight::ShaderDataType::Float4:	return GL_FLOAT;
			case InSight::ShaderDataType::Mat3:		return GL_FLOAT;
			case InSight::ShaderDataType::Mat4:		return GL_FLOAT;
			case InSight::ShaderDataType::Int:		return GL_INT;
			case InSight::ShaderDataType::Int2:		return GL_INT;
			case InSight::ShaderDataType::Int3:		return GL_INT;
			case InSight::ShaderDataType::Int4:		return GL_INT;
			case InSight::ShaderDataType::Bool:		return GL_BOOL;
		}
		EN_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	float* VertexToFloatArray(const Application::Vertex* aVertex, const int& aCount)
	{
		int count = aCount;
		float* vertices = new float[aCount * 14];
		for (int i = 0; i < count; i++)
		{
			vertices[i * 14 + 0] = aVertex[i].Position.x;
			vertices[i * 14 + 1] = aVertex[i].Position.y;
			vertices[i * 14 + 2] = aVertex[i].Position.z;
			vertices[i * 14 + 3] = aVertex[i].Position.w;
						 
			vertices[i * 14 + 4] = aVertex[i].Colour.r;
			vertices[i * 14 + 5] = aVertex[i].Colour.g;
			vertices[i * 14 + 6] = aVertex[i].Colour.b;
			vertices[i * 14 + 7] = aVertex[i].Colour.a;
						 
			vertices[i * 14 + 8] =  aVertex[i].Normal.x;
			vertices[i * 14 + 9] =  aVertex[i].Normal.y;
			vertices[i * 14 + 10] = aVertex[i].Normal.z;
			vertices[i * 14 + 11] = aVertex[i].Normal.w;
						 
			vertices[i * 14 + 12] = aVertex[i].TexCoord1.x;
			vertices[i * 14 + 13] = aVertex[i].TexCoord1.y;
		}

		return &vertices[0];
	}

	Application::Application()
		: m_running(false)
	{
		EN_CORE_ASSERT(!sInstnace, "Application already exists!");
		sInstnace = this;
	}

	bool Application::create(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
	{
		//start the logger
		Engine::Log::Init();

		mWindow = std::unique_ptr<Window>(Window::Create(WindowProps(a_name, a_width, a_height)));
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

		mImGuiLayer = new InSight::ImGuiLayer();
		PushOverlay(mImGuiLayer);

		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		//float vertices[3 * 14] =
		//{
		//	-0.5, -0.5, 0.0f, 1.0f,	  /*Colour*/ 0.8f, 0.2f, 0.8f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 1.0f, 0.0f,
		//	0.5f, -0.5f, 0.0f, 1.0f,  /*Colour*/ 0.2f, 0.3f, 0.8f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 0.0f, 0.0f,
		//	0.0f, 0.5f, 0.0f, 1.0f,   /*Colour*/ 0.8f, 0.8f, 0.2f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 1.0f, 1.0f
		//};

		Vertex vertices[3] = 
		{
			Vertex(-0.5, -0.5, 0.0f, 1.0f,	  /*Colour*/ 0.8f, 0.2f, 0.8f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.0f, 1.0f,  /*Colour*/ 0.2f, 0.3f, 0.8f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 0.0f, 0.0f),
			Vertex(0.0f, 0.5f, 0.0f, 1.0f,   /*Colour*/ 0.8f, 0.8f, 0.2f, 1.0f, /*Normals*/ 0.0f, 0.0f, 1.0f, 0.0f, /*TexCoord1*/ 1.0f, 1.0f),
		};

		float* verticesP = VertexToFloatArray(&vertices[0], 3);
 		mVertexBuffer.reset(InSight::VertexBuffer::Create(verticesP, sizeof(vertices)));
		delete[] verticesP;

		{
			BufferLayout layout =
			{
				{ShaderDataType::Float4, "aPosition"},
				{ShaderDataType::Float4, "aColour"},
				{ShaderDataType::Float4, "aNormal", true},
				{ShaderDataType::Float2, "aTexCoord1", true},
			};
			mVertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = mVertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), 
									ShaderDataTypeToOpenGLBaseType(element.Type), 
									element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}


		uint32_t indices[3] = { 0,1,2 };

		mIndexB.reset(InSight::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
		#version 330

		layout(location = 0) in vec4 aPosition;		
		layout(location = 1) in vec4 aColour;		
		layout(location = 2) in vec4 aNormal;		
		layout(location = 3) in vec2 aTexCoord1;		
		
		out vec4 vPosition;
		out vec4 vColour;

		void main()
		{
			vPosition = aPosition;
			vColour = aColour;
			gl_Position = vec4(aPosition.xyz, 1.0);
		}
	)";

		std::string fragSrc = R"(
		#version 330

		in vec4 vPosition;
		in vec4 vColour;
		layout(location = 0) out vec4 outColour;		

		void main()
		{
			outColour = vec4(vPosition.xyz * 0.5 + 0.5, 1.0);
			outColour = vColour;
		}
	)";

		mShader.reset(new InSight::Shader(vertexSrc, fragSrc));

#if DRAFT_ENABLED
		//Set Up IMGUI
		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		//Setup Dear ImGui style
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 400";

		//Setup Platform/BaseRenderer bindings
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
#endif

#ifdef __GL_DEBUG__
		glDebugMessageCallback(glErrorCallback, NULL);
#endif

		m_showFrameData = true;

		bool result = onCreate();
		if (result == false)
		{
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
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				float deltaTime = Utility::tickTimer();

				// Start the Dear ImGui frame
#if DRAFT_ENABLED
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				//ImGui Set up Framerate window
				showFrameData(true);

				mImGuiLayer->Begin();

				Update(deltaTime);

				Draw();


				for (Layer* layer : mLayerStack)
				{
					//layer->OnUpdate();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
				mImGuiLayer->Begin();

				mShader->Bind();
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, mIndexB->GetCount(), GL_UNSIGNED_INT, nullptr);

				//ImGui Set up Framerate window
				showFrameData(false);

				Update(deltaTime);

				Draw();

				for (Layer* layer : mLayerStack)
				{
					//layer->OnUpdate();
				}

				for (Layer* layer : mLayerStack)
				{
					layer->OnImGuiRender();
				}

				mImGuiLayer->End();
#endif

				mWindow->OnUpdate();
				//glfwSwapBuffers(m_window);
				//glfwPollEvents();

			} while (m_running == true);

			Destroy();
		}

		Application_Log::Destroy();
		Gizmos::destroy();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		//glfwDestroyWindow(mWindow->GetWindow());
		//glfwTerminate();
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
		return;
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
}