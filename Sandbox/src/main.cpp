#include "MyApplication.h"
#include "Sandbox.h"

#include <iostream>
#include <fstream>

#define DEFAULT_SCREENWIDTH 1920
#define DEFAULT_SCREENHEIGHT 1080

#define PreMadeScene true

// main that controls the creation/destruction of an application
int main(int argc, char* argv[])
{
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

	std::ofstream file;
	file.open("text.txt");
	if (!file.is_open())
	{
		file.open("text.txt", std::ios::out);
	}
	file.open("text.txt", std::ios::in | std::ios::out);
	file << "Test Data!!!\n";

	file.close();

	return 0;
}