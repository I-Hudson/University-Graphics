#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <glad/glad.h>
#include "Debug.h"
#include "Platform/Window.h"
#include "Layer/LayerStack.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"

struct GLFWwindow;

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

	void	quit()	{ m_running = false; }

	void	showFrameData(bool a_showFrameData);

protected:


	bool OnWindowClose(WindowCloseEvent& aEvent);
	std::unique_ptr<Window> mWindow;
	LayerStack mLayerStack;

	virtual bool	onCreate() = 0;
	virtual void	Update(float a_deltaTime) = 0;
	virtual void	Draw() = 0;
	virtual void	Destroy() = 0;

	GLFWwindow*		m_window;
	unsigned int	m_windowWidth;
	unsigned int	m_windowHeight;
	bool			m_running;
	bool			m_showFrameData;

private:
	static Application* sInstnace;
};

#endif // __APPLICATION_H_