#include "PP/PPScreenOverlay.h"
#include "Utilities.h"

PPScreenOverlay::PPScreenOverlay()
{
	//load shaders
	unsigned int vertex = Utility::loadShader("./shaders/pp/pp_BaseVertex.glsl", GL_VERTEX_SHADER);
	unsigned int fragment = Utility::loadShader("./shaders/pp/pp_screenOverlay.glsl", GL_FRAGMENT_SHADER);

	//create progam
	const char* inputs[] = { "Position", "TexCoord1" };
	const char* outputs[] = { "FragColour" };
	mProgram = Utility::createProgram(
		vertex,
		0,
		0,
		0,
		fragment, 
		2, inputs,
		1, outputs);

	//delete BaseShader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

PPScreenOverlay::~PPScreenOverlay()
{
	//delete program
	glDeleteProgram(mProgram);
}

void PPScreenOverlay::draw()
{
	//render quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(mProgram);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
}
