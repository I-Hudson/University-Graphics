#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

class FullScreenShader : public Shader
{
public:
	//Constructors
	FullScreenShader();
	FullScreenShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~FullScreenShader();

	//override
	void useShader(bool aClear = false) override;

private:

	//fullscreen buffers
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mIBO;

	struct FullScreenVertex
	{
		glm::vec4 Position;
		glm::vec2 TexCoord;

		FullScreenVertex() {}
		FullScreenVertex(glm::vec4 aPosition, glm::vec2 a_texCorrd) : Position(aPosition), TexCoord(a_texCorrd) {}
	};


};