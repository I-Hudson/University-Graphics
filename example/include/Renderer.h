#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Shader.h"

#include <vector>

class Renderer
{
private:
	//store all the enitity to draw
	std::vector<Shader*> mShadersToRender;
	//when a mesh component is added. Add that to this vector to 
	//draw 

public:
	Renderer();
	~Renderer();

	//add entity
	void addShader(Shader* a_addShader);

	//remove entity
	void removeShader(Shader* a_removeShader);

	//draw
	void draw();
};

#endif //__RENDERER_H__
