#include "Debug.h"
#include "MyApplication.h"
#include "Sandbox.h"

#define DEFAULT_SCREENWIDTH 1920
#define DEFAULT_SCREENHEIGHT 1080

#define PreMadeScene true

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (!PreMadeScene)
	{
		Sandbox* app = new Sandbox();
		app->run("", 1024, 720, false);

		// explicitly control the destruction of our application
		delete app;
	}
	else
	{
		// explicitly control the creation of our application
		MyApplication* app = new  MyApplication();
		app->run("InSIght", DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, false);

		// explicitly control the destruction of our application
		delete app;
	}

	return 0;
}