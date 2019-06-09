#pragma once

#include <map>

#include "Shader/BaseShader.h"

class DeferredShader : public BaseShader
{
public:
	//Constructors
	DeferredShader();
	DeferredShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destructor
	~DeferredShader();

	//get framebuffer
	unsigned int*	getFrameBuffer() { return &mFrameBuffer; }

	//overrides
	void			useShader(bool aClear = false) override;
	void			gui() override;

private:
	//render this BaseShader in wireframe
	bool mRenderWireFrame;
};