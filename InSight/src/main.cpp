#include "MyApplication.h"

#define DEFAULT_SCREENWIDTH 1920
#define DEFAULT_SCREENHEIGHT 1080

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
	// explicitly control the creation of our application
	MyApplication* app = new  MyApplication();
	app->run("CT6025_IanHudson", DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, false);

	// explicitly control the destruction of our application
	delete app;

	return 0;
}